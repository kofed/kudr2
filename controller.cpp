#include "controller.h"
#include <sstream>

Controller::Controller()
{

}

void Controller::init(){
    //pings from 192.168.0.0 to 192.168.16.1
    try{
     //   pingController.scan();

        if(pingController.activeIps.size() < 1){
            return;
        }

        setCameras(CameraIp(* pingController.activeIps.begin()));
    }catch(std::exception & e){

    }
}

void Controller::saveROI(const CameraIp * camera, const int x1, const int y1, const int x2, const int y2){
    if(camera == NULL)
        return;

    stringstream ss;
    ss << "echo \"";
    ss << x1 << "\n";
    ss << y1 << "\n";
    ss << x2 << "\n";
    ss << y2 << "\"";
    ss << " > /tmp/roi.xml";

    sshController.init(camera->toString(), "pi", "raspberry");
    sshController.command(QString(ss.str().c_str()));
    sshController.shutdown();
}

void Controller::loadDebug(const CameraIp::CameraPosition position){

}

void Controller::loadShot(const CameraIp * camera, const int width, const int height){
    if(camera == NULL || width < 0 || height < 0)
        return;

    QString file = imagePattern.arg(camera->getPosition());
    sshController.init(camera->toString(), "pi", "raspberry");
    sshController.command(QString("raspistill -e png -w %1 -h %2 -o %3").arg(width).arg(height).arg(file));
    sshController.file(file, file);
    sshController.shutdown();
}

void Controller::cameraOn(const CameraIp::CameraPosition position){

}

void Controller::saveDoorNum(const int value){
    saveDoorNum(lCamera, value);
    saveDoorNum(rCamera, value);
}

void Controller::saveDoorNum(CameraIp * camera, const int value){
    if(camera == NULL)
        return;

    sshController.init(camera->toString(), "pi", "raspberry");
    camera->setDoorNumber(value);
    sshController.command(QString("sed -i 's/address *.*.*.*/address %1/g' /etc/network/interfaces").arg(camera->toString()));
    sshController.command(QString("sed -i 's/ip_address=*.*.*.*/ip_address=%1/g' /etc/dhcpcd.conf").arg(camera->toString()));
    sshController.command("sudo /etc/init.d/networking restart");
    sshController.shutdown();
}

void Controller::searchOnDoorNum(int doorNum){
    CameraIp _lcamera(doorNum, CameraIp::left);
    CameraIp _rcamera(doorNum, CameraIp::right);

    setCameras(_lcamera, _rcamera);

}

void Controller::setCameras(const CameraIp _camera){
    CameraIp opCamera = _camera.buildOpposite();
    if(_camera.getPosition() == CameraIp::left){
        setCameras(_camera, opCamera);
    }else{
        setCameras(opCamera, _camera);
    }
}

void Controller::setCameras(const CameraIp _lCamera, const CameraIp _rCamera){
    if(lCamera != NULL){
        delete lCamera;
    }

    if(rCamera != NULL){
        delete rCamera;
    }

    if(pingController.ping(_lCamera.toString()) == 0){
        lCamera = new CameraIp(_lCamera.toString());
    }

    if(pingController.ping(_rCamera.toString()) == 0){
        rCamera = new CameraIp(_rCamera.toString());
    }
}
