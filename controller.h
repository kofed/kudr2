#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "CameraIp.h"
#include "sshcontroller.h"
#include "pingcontroller.h"
#include "CameraIp.h"
#include "logger.h"

/**
 * Контроллер графического интерфейса. Модель
 */
class Controller
{

public:
    Controller(Logger* & _logger);

    void saveROI(const Position pos, const int resolutionWidth, const int resolutionHeight, const int x1, const int y1, const int x2, const int y2);

    void loadDebug(const Position position);

    void loadShot( );

    void searchOnDoorNum(const int doorNum);

    void saveCamera(const Position pos , const CameraIp & newCamera);

    void setCamera(const Position pos, const CameraIp & camera);

    void setCameras(const CameraIp & camera);

    bool hasCameras();

    QString getImgFileName(const Position position) const;

    QString imagePattern = "/tmp/shot%1.png";

    int width = 400, height = 300;

    CameraIp * cameras[2];

    friend class MainWindow;
private:

    SShController sshController;

    PingController pingController;

    Logger* & logger;

signals:

    void shotUpdated(){}

    void cameraChanged(){}
};

#endif // CONTROLLER_H
