#include "controller.h"

Controller::Controller()
{

}

void Controller::init(){
    //pings from 192.168.0.0 to 192.168.16.1
 //   pingController.scan();

    if(pingController.activeIps.size() < 1){
        return;
    }

    QString ip = * pingController.activeIps.begin();
    QString opIp = CameraIp::getOppositeIp(ip);
    if(CameraIp::getPosition(ip) == CameraIp::CameraPosition::left){
        lIp = ip;
        if(pingController.activeIps.find(opIp) != pingController.activeIps.end()){
            rIp = CameraIp::getOppositeIp(ip);
        }
    }else{
        rIp = ip;
        if(pingController.activeIps.find(opIp) != pingController.activeIps.end()){
            lIp = CameraIp::getOppositeIp(ip);
        }
    }

}

void Controller::saveROI(const CameraIp::CameraPosition position){

}

void Controller::loadDebug(const CameraIp::CameraPosition position){

}

void Controller::loadShot(const CameraIp::CameraPosition position){

}

void Controller::cameraOn(const CameraIp::CameraPosition position){

}

void Controller::saveDoorNum(const int value){

    sshController.init(QString("192.168.%1.0").arg(value), "pi", "raspberry");
    sshController.command("");
    sshController.shutdown();
}
