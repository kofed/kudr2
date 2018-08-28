#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "CameraIp.h"
#include "sshcontroller.h"
#include "pingcontroller.h"
#include "CameraIp.h"

class Controller
{


public:
    Controller();

    void init();

    void saveROI(const CameraIp::CameraPosition position);

    void loadDebug(const CameraIp::CameraPosition position);

    void loadShot(const CameraIp::CameraPosition position);

    void cameraOn(const CameraIp::CameraPosition position);

    void saveDoorNum(const int value);

    QString lIp;
    QString rIp;
private:
    PingController pingController;

    SShController sshController;

};

#endif // CONTROLLER_H
