#ifndef CONST_H
#define CONST_H

#include <QString>
#include <array>

using namespace std;

class CameraIp{

public:
    CameraIp(const QString & _ip);

    CameraIp(const array<int, 4> ip);

    enum CameraPosition {left, right, notSet};

    CameraPosition getPosition() const;

    QString toString() const;

    CameraIp buildOpposite();

    int getDoorNumber();

    void setDoorNumber(const int number);
private:

    array<int, 4> ip;

    CameraPosition position;


};


#endif // CONST_H
