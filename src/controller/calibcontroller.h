#ifndef CVCONTROLLER_H
#define CVCONTROLLER_H

#include <vector>
#include "opencv2/opencv.hpp"
#include <map>
#include "position.h"
#include "controller.h"
#include "calibData.h"

using namespace cv;
using namespace std;

class SpiralIterator{

public:
    SpiralIterator(Point2i _centerIndex, const Size _size)
        :centerIndex(_centerIndex), step(1), phi(0),  iX(0), iY(0), size(_size){

    }

    bool next();

    int getIX();

    int getIY();

private:
    Point2i centerIndex;

    //отступ от центра в клетках
    int step;
    //шаг отсутупа
    double phi;
    //абсолютные индексы для итерации вокруг центра
    int iX;
    int iY;

    const Size size;

};

class CalibController
{
    // кеш калибровачный данных
	CalibData cache;

    //координаты углов на камере слева/справа
    map<Position, vector<vector<Point2f>>> corners; 
    const Controller & controller;

public:
    map<Position, Point2i> centers;
    map<Position, Rect> & rois;
    map<Position, Mat> images;

    map<Position, Size> sizes;

    CalibController(const Controller & _controller, map<Position, Rect> & _rois);

    void findChessboardCorners();

    void deleteCorners();

    void addCalibEntities(const int h, const int cellSize);

    void saveYML();

    void sendYML();

    void openImage(const Position pos, const QString & path);

    Mat getImageWithCorners(const Position pos);

    void addCorner(const Position pos, const Point2f corner);

    void deleteCorner(const Position pos, const Point2f corner);

    int patternWasFound(Position pos);    

private:
    vector<vector<Point2f>> findChessboardCorners(Mat & image,const Size & size);

    Point2i findClosestCornerIndexSorted(const Point2f & point, const Position & pos);

    vector<Point2f>::iterator findClosestCorner(const Point2f & point, const Position & pos);

    void sortCorners(const Position pos);
};

#endif // CVCONTROLLER_H
