#include "CameraIp.h"
#include <QStringList>
#include <sstream>
#include <QtDebug>

CameraIp::CameraIp(const QString & _ip){
    QStringList list = _ip.split('.');

    for(int i = 0; i < 4; ++i){
        ip[i] = list[i].toInt();
    }


}

CameraIp::CameraIp(array<int, 4> _ip){
    ip = _ip;
    switch(ip[3]){
    case 0:
        position = CameraIp::CameraPosition::left;
        break;
    case 1:
        position = CameraIp::CameraPosition::right;
        break;
    default:
        position = CameraIp::CameraPosition::notSet;
        break;
    }
    qDebug() << "built cameraIp with ip = " <<  toString();
}

CameraIp::CameraPosition CameraIp::getPosition() const {
    return position;
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

CameraIp CameraIp::buildOpposite(){
    array<int, 4> opIp = ip;
    switch(position){
    case CameraIp::CameraPosition::left:
        opIp[3] = CameraIp::CameraPosition::right;
        break;
    case CameraIp::CameraPosition::right:
        opIp[3] = CameraIp::CameraPosition::left;
        break;
    }

    return CameraIp(opIp);
}

int CameraIp::getDoorNumber(){
    return ip[2];
}

void CameraIp::setDoorNumber(const int number){
    ip[2] = number;
}
