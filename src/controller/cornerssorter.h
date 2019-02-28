#ifndef CORNERSSORTER_H
#define CORNERSSORTER_H

#include <vector>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

class CornerSorter{
public:
    CornerSorter(vector<Point2f> & _corners, const Size & _size);

    vector<Point2f> sort();
private:
    vector<Point2f> & corners;

    const Size size;

    float findRadius();

    int findTopLeft();

    Point2f findBotRight();

    int findNext(const int  point );

 //   void swap(const int i, const int j);

    float dist(const int i, const int j);
};

#endif // CORNERSSORTER_H
