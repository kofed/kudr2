#include "controller.h"
#include <sstream>
#include <QDebug>

Controller::Controller()
{
    cameras[0] = NULL;
    cameras[1] = NULL;
}

void Controller::saveROI(const Position pos,  const int resolutionWidth, const int resolutionHeight,
                         const int x1, const int y1, const int x2, const int y2){
    if(cameras[pos] == NULL)
        return;

    stringstream ss;
    ss << "echo \"";
    ss << resolutionWidth << "\n";
    ss << resolutionHeight << "\n";
    ss << x1 << "\n";
    ss << y1 << "\n";
    ss << x2 << "\n";
    ss << y2 << "\"";
    ss << " > /tmp/roi.xml";

    sshController.init(cameras[pos]->toString(), "pi", "raspberry");
    sshController.command(QString(ss.str().c_str()));
    sshController.shutdown();
}

void Controller::loadDebug(const Position position){

}

void Controller::loadShot(const Position pos, const int width, const int height){
    if(cameras[pos] == NULL || width < 0 || height < 0)
        return;

    QString file = imagePattern.arg(pos);
    sshController.init(cameras[pos]->toString(), "pi", "raspberry");
    try{
        sshController.command(QString("raspistill -e png -w %1 -h %2 -o %3").arg(width).arg(height).arg(file));
        sshController.file(file, file);
        sshController.command(QString("rm %1").arg(file));
    }catch(std::exception & e){
        sshController.shutdown();
        throw e;
    }
    sshController.shutdown();
}

void Controller::saveCamera(const Position pos, const CameraIp & cameraNew){
    if(cameras[pos] == NULL)
        return;

    sshController.init(cameras[pos]->toString(), "pi", "raspberry");
    sshController.command(QString("sed -i 's/address *.*.*.*/address %1/g' /etc/network/interfaces").arg(cameraNew.toString()));
    sshController.command(QString("sed -i 's/ip_address=*.*.*.*/ip_address=%1/g' /etc/dhcpcd.conf").arg(cameraNew.toString()));
    sshController.command("sudo /etc/init.d/networking restart");
    sshController.shutdown();

    setCamera(pos, cameraNew);
}

void Controller::setCamera(const Position pos, const CameraIp & camera){
    delete cameras[pos];

    if(pingController.ping(camera.toString()) != 0){
        qDebug() << (QString("ненайдено устройство с ip=%1").arg(camera.toString()));
        return;
    }

    cameras[pos] = new CameraIp(camera.toString());

}

void Controller::setCameras(const CameraIp & camera){
    setCamera(camera.getPosition(), camera);
    CameraIp opCamera = camera.buildOpposite();
    setCamera(opCamera.getPosition(), opCamera);
}
