#ifndef CONST_H
#define CONST_H

#include <QString>

using namespace std;

class CameraIp{

public:
    CameraIp(const QString & _ip);

    enum CameraPosition {left, right};

    CameraPosition getPosition();

    static CameraPosition getPosition(QString ip);

    static QString getOppositeIp(QString ip);
private:

    QString ip;


};


#endif // CONST_H
