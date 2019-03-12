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

            const Point2f point;

            Corner(const Point2f & point);
            Corner* getNeighbor(const Neighborhood n);
            void setNeighbor(const Neighborhood n, Corner* corner);
            bool checkNeighborhood(const Neighborhood n, const Corner* corner);

            void findAndSetNeighborhood(
                vector<Corner>::iterator neibBegin,
                vector<Corner>::iterator  neibEnd);

            vector<Corner> removeOutstanging(vector<Point2f> corners);
            float dist(const Corner* c);

            Neighborhood findNeighborhood(const Corner* c);

        private:

            map<Neighborhood, Corner*> neighbors;
            map<Neighborhood, float> distances;//DEBUG
            map<Corner*, float> corner2tgs; //DEBUG
            map<Corner*, float> corner2dist;//DEBUG

        };
    vector<Corner> corners;
    Corner* findTopLeft();
    vector<Corner*> getRow(Corner * start);


public:
    ChessBoard(const Size & size, const vector<Point2f> & corners);

    vector<vector<Point2f>> getSortedCornerPoints();
    vector<vector<Corner*>> getSortedCorners();
    vector<Point2f> getSortedCornerPoints1D();
};

#endif // CORNERSSORTER_H
