#ifndef CVCONTROLLER_H
#define CVCONTROLLER_H

#include <vector>
#include "opencv2/opencv.hpp"
#include <map>
#include "position.h"
#include "controller.h"

using namespace cv;
using namespace std;

class SpiralIterator{

public:
    SpiralIterator(Point2i _centerIndex, const Size _size)
        :step(1),  phi(0), size(_size), centerIndex(_centerIndex), iX(0), iY(0){

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


class CalibCorner{
    //угол на левой/правой камере
    const Point2f pointL, pointR;
    //местоположение точки относительно центра
    const Point2f p;
public:
    CalibCorner(const Point2f & _pointL, const Point2f & _pointR, const Point2i & center);

    void toYml(FileStorage & yml) const {
        yml << "pointL" << pointL;
        yml << "pointR" << pointR;
        yml << "p" << p;
    }
};

//модель для сохранения в yml
class CalibShot{
public:
    //расстояние между плоскостями введенное оператором
    const int h;
    //размер клетки шахматной доски мм
    const int cellSize;
    vector<CalibCorner> corners;

    CalibShot(const int _h, const int _cellSize):h(_h), cellSize(_cellSize){

    }

    void toYml(FileStorage & yml){
        yml << "h" << h;
        yml << "cellSize" << cellSize;
        for(auto c : corners){
            c.toYml(yml);
        }
    }
};


class CalibController
{
    // кеш калибровачный данных
    vector<CalibShot> cache;
    //координаты углов на камере слева/справа
    map<Position, vector<vector<Point2f>>> corners; 
    const Controller & controller;

public:
    map<Position, Point2i> centers;
    map<Position, Rect> & rois;
    map<Position, Mat> images;

    map<Position, Size> sizes;

    CalibController(const Controller & _controller, map<Position, Rect> & _rois);

    void calibrateCamera();

    void findChessboardCorners();

    void deleteCorners();

    void addCalibEntities(const int h, const int cellSize);

    void saveYML();

    void sendYML();

    void openImage(const Position pos, const QString & path);

    Mat getImageWithCorners(const Position pos);

    void addCorner(const Position pos, const Point2i corner, const Size index);

    void deleteCorner(const Position pos, const Point2i corner);

private:
    vector<vector<Point2f>> findChessboardCorners(Mat & image,const Size & size);

    Point2i findClosestCornerIndex(const Point2i & point, const Position & pos);

    bool patternWasFound(Position pos);

    void sortCorners(const Position pos);
};

#endif // CVCONTROLLER_H
