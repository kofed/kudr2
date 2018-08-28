#include "CameraIp.h"
#include <QStringList>

CameraIp::CameraIp(const QString & _ip):ip(_ip){

}

CameraIp::CameraPosition CameraIp::getPosition(){
    return getPosition(ip);
}

CameraIp::CameraPosition CameraIp::getPosition(QString _ip){
    QStringList list = _ip.split('.');

    if(list.value(2) == CameraIp::CameraPosition::left){
        return CameraIp::CameraPosition::left;
    }

    return CameraIp::CameraPosition::right;
}

QString CameraIp::getOppositeIp(const QString ip){
    QStringList list = ip.split('.');
    if(list.size() != 4){
        return ip;
    }

    if(list.at(3) == CameraIp::CameraPosition::left){
        list.replace(3, QString(CameraIp::CameraPosition::right));
    }

    if(list.at(3) == CameraIp::CameraPosition::right){
        list.replace(3, QString(CameraIp::CameraPosition::left));
    }

    return list.join('.');
}
