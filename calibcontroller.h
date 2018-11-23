#ifndef CVCONTROLLER_H
#define CVCONTROLLER_H

#include <vector>
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

class CalibController
{
public:
    CalibController(const Size & patternSize);

    void drawChessBoardCorners(Mat & image, const vector<Point> & corners);

    void calibrateCamera();
};

#endif // CVCONTROLLER_H
