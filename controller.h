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

    void saveROI(const CameraIp camera, const int x1, const int y1, const int x2, const int y2);

    void loadDebug(const CameraIp::CameraPosition position);

    void loadShot(const CameraIp position);

    void cameraOn(const CameraIp::CameraPosition position);

    void saveDoorNum(const int value);

    void saveDoorNum(const CameraIp camera, const int value);

    CameraIp lCamera = CameraIp("0.0.0.0");
    CameraIp rCamera = CameraIp("0.0.0.0");

    QString imagePattern = "/tmp/shot%d.png";
private:
    PingController pingController;

    SShController sshController;

};

#endif // CONTROLLER_H
