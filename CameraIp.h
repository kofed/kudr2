#ifndef CONST_H
#define CONST_H

#include <QString>
#include <array>

using namespace std;

class CameraIp{

public:
    const static QString IP_MASK;

    CameraIp(const QString & _ip);

    CameraIp(const array<int, 4> ip);

    enum CameraPosition {left, right, notSet};

    CameraIp(const int doorNum, const CameraPosition position);

    CameraPosition getPosition() const;

    QString toString() const;

    CameraIp buildOpposite() const;

    int getDoorNumber() const;

    void setDoorNumber(const int number);
private:

    array<int, 4> ip;

    int doorNum;
};


#endif // CONST_H
