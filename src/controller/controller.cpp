#include "controller.h"
#include <sstream>
#include <QDebug>

Controller::Controller()
{
    cameras[0] = NULL;
    cameras[1] = NULL;

    images[LEFT] = "/tmp/shot0.png";
    images[RIGHT] = "/tmp/shot1.png";
}

void Controller::saveROI(){
    for(auto pos : positions){
        Rect & roi = rois[pos];
        if(cameras[pos] == NULL || roi.width <= 0 || roi.height <= 0)
            return;

        Logger::log("Сохраняю рабочую область на устройство");

        stringstream ss;
        ss << "echo \"";
        ss << resolution.width << "\n";
        ss << resolution.height << "\n";
        ss << roi.x << "\n";
        ss << roi.y << "\n";
        ss << roi.width << "\n";
        ss << roi.height << "\"";
        ss << " > /tmp/roi.xml";

        sshController.init(cameras[pos]->toString(), "pi", "raspberry");
        sshController.command(QString(ss.str().c_str()));
        sshController.shutdown();

        Logger::log("Рабочая область сорхранена");
    }

}

void Controller::loadDebug(const Position position){

}

void Controller::loadShot( ){
    if(cameras[LEFT] == NULL && cameras[RIGHT] == NULL || resolution.width < 0 || resolution.height < 0)
        throw std::runtime_error("Произведите поиск и выберите устройство");

    for(auto pos : positions){
        if(cameras[pos] == NULL){
            continue;
        }
        QString & file = images[pos];
        QString ip = cameras[pos]->toString();
        sshController.init(ip, "pi", "raspberry");
        try{
            sshController.command(QString("raspistill -e png -w %1 -h %2 -o %3").arg(resolution.width).arg(resolution.height).arg(file));
            sshController.fileFrom(file, file);
            sshController.command(QString("rm %1").arg(file));
        }catch(std::exception & e){
            Logger::log(QString("Невозможно получить снимок для %1").arg(ip));
        }

         sshController.shutdown();
    }

}

bool Controller::hasCameras(){
    return cameras[LEFT] != NULL || cameras[RIGHT] != NULL;
}

void Controller::saveCamera(const Position pos, const CameraIp & cameraNew){
    if(cameras[pos] == NULL)
        return;

    sshController.init(cameras[pos]->toString(), "pi", "raspberry");
    sshController.command(QString("sudo sed -i 's/address *.*.*.*/address %1/g' /etc/network/interfaces").arg(cameraNew.toString()));
    sshController.command(QString("sudo sed -i 's/ip_address=*.*.*.*/ip_address=%1/g' /etc/dhcpcd.conf").arg(cameraNew.toString()));
    sshController.command("sudo reboot");
    sshController.shutdown();

    delete cameras[pos];
    cameras[pos] = NULL;
}

void Controller::setCamera(const Position pos, const CameraIp & camera){
    delete cameras[pos];

    if(pingController.ping(camera.toString()) != 0){
        Logger::log(QString("ненайдено устройство с ip=%1").arg(camera.toString()));
        return;
    }

    cameras[pos] = new CameraIp(camera.toString());
    Logger::log(QString("найдено устройство с ip=%1").arg(camera.toString()));

}

void Controller::setCameras(const CameraIp & camera){
    setCamera(camera.getPosition(), camera);
    CameraIp opCamera = camera.buildOpposite();
    setCamera(opCamera.getPosition(), opCamera);

    emit cameraChanged();
}


