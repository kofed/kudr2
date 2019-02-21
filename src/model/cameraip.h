#ifndef CONST_H
#define CONST_H

#include <QString>
#include <array>
#include "position.h"

using namespace std;

class CameraIp{

public:
    const static QString IP_MASK;

    CameraIp(const QString & _ip);

    CameraIp(const array<int, 4> ip);

    CameraIp(const int doorNum, const Position position);

    Position getPosition() const;

    //void setCameraPosition(const CameraPosition posiotn);

    QString toString() const;

    CameraIp buildOpposite() const;

    int getDoorNumber() const;

    //void setDoorNumber(const int number);
private:

    array<int, 4> ip;
};


#endif // CONST_H
