#ifndef CVCONTROLLER_H
#define CVCONTROLLER_H

#include <vector>
#include "opencv2/opencv.hpp"
#include <map>
#include "position.h"
#include "controller.h"

using namespace cv;
using namespace std;

class ChessBoardCenterIterator{

public:
    ChessBoardCenterIterator(Point2i _center):step(1), phi(0), iX(_center.x), iY(_center.y){

    }

    bool next();

    int getIX();

    int getIY();

private:
    //отступ от центра в клетках
    int step;
    //шаг отсутупа
    int phi;
    //абсолютные индексы для итерации вокруг центра
    int iX;
    int iY;

};

class CalibEntity{

    //расстояние между плоскостями введенное оператором
    const int h;
    //угол на левой/правой камере
    const Point2f pointL, pointR;
    //disparity расчетное
    //int disparity;

public:
    CalibEntity(const int _h, const Point2f & _pointL, const Point2f & _pointR)
        :h(_h), pointL(_pointL), pointR(_pointR){

    }

    //CalibEntity(){}

    void toYml(FileStorage & yml) const {
        yml << "h" << h;
        yml << "pointL" << pointL;
        yml << "pointR" << pointR;
    }
};

class CalibController
{
    // кеш калибровачный данных
    vector<CalibEntity> cache;

    //координаты углов на камере слева/справа
    map<Position, vector<vector<Point2f>>> corners;

    const Controller & controller;

public:
    map<Position, Point2i> centers;
    map<Position, Size> sizes;

    CalibController(Controller & _controller):controller(_controller){}

    void calibrateCamera();

    void findChessboardCorners();

    void deleteCorners();

    void addCalibEntities(const Point2i & lCenterIndex, const Point2i & rCenterIndex, const int h);

    void saveYML();

    void sendYML();

private:
    vector<vector<Point2f>> findChessboardCorners(Mat & image,const Size & size);

    Point2i findClosestCornerIndex(const Point2i & point, const Position & pos);
};

#endif // CVCONTROLLER_H
