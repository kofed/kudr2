#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "CameraIp.h"
#include "sshcontroller.h"
#include "pingcontroller.h"
#include "CameraIp.h"
#include "logger.h"

class Controller
{

public:
    Controller(Logger* & _logger);

    void saveROI(const Position pos, const int resolutionWidth, const int resolutionHeight, const int x1, const int y1, const int x2, const int y2);

    void loadDebug(const Position position);

    void loadShot(const Position pos, const int width, const int height);

    void searchOnDoorNum(const int doorNum);

    void saveCamera(const Position pos , const CameraIp & newCamera);

    void setCamera(const Position pos, const CameraIp & camera);

    void setCameras(const CameraIp & camera);

    QString imagePattern = "/tmp/shot%1.png";

    int lWidth, lHeight, rWidth, rHeight;

    friend class MainWindow;
private:
    CameraIp * cameras[2];

    PingController pingController;

    SShController sshController;

    Logger* & logger;
};

#endif // CONTROLLER_H
