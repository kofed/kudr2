#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "cameraip.h"
#include "sshcontroller.h"
#include "pingcontroller.h"
#include "logger.h"
#include "opencv2/opencv.hpp"

using namespace cv;

/**
 * Контроллер графического интерфейса. Модель
 */
class Controller
{

public:
    Size resolution = Size(400, 600);
    map<Position, Rect> rois;

    //изображения для калибровки
    map<Position, QString> images;

    CameraIp * cameras[2];

    Controller();

    void saveROI();

    void loadDebug(const Position position);

    void loadShot( );

    void searchOnDoorNum(const int doorNum);

    void saveCamera(const Position pos , const CameraIp & newCamera);

    void setCamera(const Position pos, const CameraIp & camera);

    void setCameras(const CameraIp & camera);

    bool hasCameras();

   // QString getImage(Position p)const{
   //     return images.at(p);
   // }

    friend class MainWindow;
private:

    SShController sshController;

    PingController pingController;

signals:

    void shotUpdated(){}

    void cameraChanged(){}
};

#endif // CONTROLLER_H
