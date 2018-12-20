#ifndef CVCONTROLLER_H
#define CVCONTROLLER_H

#include <vector>
#include "opencv2/opencv.hpp"
#include <map>
#include "position.h"

using namespace cv;
using namespace std;

class CalibEntity{

    vector<vector<cv::Point2f>> corners;

    int distance;

public:
    CalibEntity(const int _distance, vector<vector<cv::Point2f>> & _corners)
        :distance(_distance), corners(_corners){

    }

    CalibEntity(){}

    void toYml(FileStorage & yml){
        yml << distance;
        yml << corners;
    }
};

class CalibController
{
    map<Position, vector<CalibEntity>> cache;

    map<Position, CalibEntity> lastCorners;

public:
    CalibController();

    void calibrateCamera();

    vector<vector<cv::Point2f>> & findChessboardCorners(Position pos, Mat & image,const Size & size);

    void deleteCorners(const int pos);

    void addCorners(const int distance);

    void saveYML();
};

#endif // CVCONTROLLER_H
