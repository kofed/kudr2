#include "cameraip.h"
#include <QStringList>
#include <sstream>
#include <QtDebug>

const QString CameraIp::IP_MASK = QString("192.168.43.%1%2");

CameraIp::CameraIp(const QString & _ip){
    QStringList list = _ip.split('.');

    for(int i = 0; i < 4; ++i){
        ip[i] = list[i].toInt();
    }

    qDebug() << "built cameraIp with ip = " <<  toString();
}

CameraIp::CameraIp(array<int, 4> _ip){
    ip = _ip;
    qDebug() << "built cameraIp with ip = " <<  toString();
}

CameraIp::CameraIp(const int doorNum, const Position position){
    QString ipString = IP_MASK.arg(doorNum).arg(position);

    QStringList list = ipString.split('.');

    for(int i = 0; i < 4; ++i){
        ip[i] = list[i].toInt();
    }

}

Position CameraIp::getPosition() const {

    switch(ip[3] % 10){
    case 0:
        return LEFT;
    case 1:
        return RIGHT;
    default:
        return NOTSET;
    }
}


QString CameraIp::toString() const {
    stringstream ss;
    for(int i = 0; i < 4; ++i){
        ss << ip[i];
        if(i != 3){
            ss << ".";
        }
    }

    return QString(ss.str().c_str());
}

CameraIp CameraIp::buildOpposite() const{
    array<int, 4> opIp = ip;
    switch(getPosition()){
    case LEFT:
        opIp[3] = 10 * getDoorNumber() + Position::RIGHT;
        break;
    case RIGHT:
        opIp[3] = 10* getDoorNumber() + Position::LEFT;
        break;
    }

    return CameraIp(opIp);
}

int CameraIp::getDoorNumber() const {
    return ip[3]/10;
}
