#include "controller.h"
#include <sstream>

Controller::Controller()
{

}

void Controller::init(){
    //pings from 192.168.0.0 to 192.168.16.1
    pingController.scan();

    if(pingController.activeIps.size() < 1){
        return;
    }

    CameraIp camera = CameraIp(* pingController.activeIps.begin());
    CameraIp opCamera = camera.buildOpposite();
    if(camera.getPosition() == CameraIp::CameraPosition::left){
        lCamera = camera;
        if(pingController.activeIps.find(opCamera.toString()) != pingController.activeIps.end()){
            rCamera = opCamera;
        }
    }else{
        rCamera = opCamera;
        if(pingController.activeIps.find(opCamera.toString()) != pingController.activeIps.end()){
            rCamera = opCamera;
        }
    }

}

void Controller::saveROI(const CameraIp camera, const int x1, const int y1, const int x2, const int y2){
    stringstream ss;
    ss << "echo \"";
    ss << x1 << "\n";
    ss << y1 << "\n";
    ss << x2 << "\n";
    ss << y2 << "\"";
    ss << " > /tmp/roi.xml";

    sshController.init(camera.toString(), "pi", "raspberry");
    sshController.command(QString(ss.str().c_str()));
    sshController.shutdown();
}

void Controller::loadDebug(const CameraIp::CameraPosition position){

}

void Controller::loadShot(const CameraIp camera){
    QString file = imagePattern.arg(camera.getPosition());
    sshController.init(camera.toString(), "pi", "raspberry");
    sshController.command(QString("raspistill -o " + file));
    sshController.file(file, file);
    sshController.shutdown();
}

void Controller::cameraOn(const CameraIp::CameraPosition position){

}

void Controller::saveDoorNum(const int value){
    saveDoorNum(lCamera, value);
    saveDoorNum(rCamera, value);
}

void Controller::saveDoorNum(const CameraIp camera, const int value){
    sshController.init(camera.toString(), "pi", "raspberry");
    lCamera.setDoorNumber(value);
    sshController.command(QString("sed -e 's/address*.*.*.*/%s/g' /etc/network/interfaces").arg(camera.toString()));
    sshController.command(QString("sed -e 's/address*.*.*.*/%s/g' /etc/dhcpcd.conf").arg(camera.toString()));
    sshController.shutdown();
}
