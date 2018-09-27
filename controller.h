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

    void saveROI(const CameraIp * camera, const int resolutionWidth, const int resolutionHeight, const int x1, const int y1, const int x2, const int y2);

    void loadDebug(const CameraIp::CameraPosition position);

    void loadShot(const CameraIp * position, const int width, const int height);

    void saveDoorNum(const int value);

    void saveDoorNum(CameraIp * camera, const int value);

    void searchOnDoorNum(int doorNum);

    void setCameras(CameraIp camera);

    void setCameras(CameraIp , CameraIp);

    CameraIp * lCamera = NULL;
    CameraIp * rCamera = NULL;

    QString imagePattern = "/tmp/shot%1.png";

    friend class MainWindow;
private:
    PingController pingController;

    SShController sshController;

};

#endif // CONTROLLER_H
