#ifndef CORNERSSORTER_H
#define CORNERSSORTER_H

#include <vector>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

class ChessBoard{
private:


    const Size size;
    class Corner{
    private:
        Point2f point;
        vector<Corner*> neighbors;

        void setNeighborhood(vector<Corner*> corners);
    public:
        enum Neighborhood{
            LEFT,
            RIGHT,
            BOT,
            UP,
            UPLEFT,
            UPRIGHT,
            BOTLEFT,
            BOTRIGHT
        };

        Corner(Point2f & point);
        Point2f getPoint() const;
        Corner* getNeighbor(Neighborhood n);

        vector<Corner> removeOutstanging(vector<Point2f> corners);
        float dist(const Corner & c);
    };

    vector<Corner> corners;
    ChessBoard(vector<Point2f> corners, const Size & size);

    float dist(const Point2f & p1, const Point2f & p2);


};

#endif // CORNERSSORTER_H
