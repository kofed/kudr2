#ifndef CORNERSSORTER_H
#define CORNERSSORTER_H

#include <vector>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

const float PI = 3.14159265;

/**
 * класс для сортировки углов шахматной доски. Считает углы (в радианах) между углами шахматной доски
 */
class ChessBoardCornerSorter{
private:
    const Size size;

    class Corner{
        public:
            enum Neighborhood{
                LEFT,
                RIGHT,
                BOT,
                UP
            };

            const Point2f point;

            Corner(const Point2f & point);
            const Corner* getNeighbor(const Neighborhood n);
         //   void setNeighbor(const Neighborhood n, Corner* corner);
          //  bool checkNeighborhood(const Neighborhood n, const Corner* corner);

            void findAndSetNeighborhood(
                vector<Corner>::iterator neibBegin,
                vector<Corner>::iterator  neibEnd);

          //  vector<Corner> removeOutstanging(vector<Point2f> corners);
            float dist(const Corner* c);

            void trySetNeighborhood(const Corner* c);


            static bool cmp(const Corner* c1, const Corner * c2){
                return c1->point.x + c1->point.y < c2->point.x + c2->point.y;
            }
        private:

            map<Neighborhood, const Corner*> neighbors;
            map<Neighborhood, float> nbh2tg;

            void compareAndSet(Neighborhood n, const Corner* c);
            float tang(const Corner * c);
            float tgDelta(Neighborhood n, const Corner * c, const Corner * c1);

            float hashDelta(Neighborhood n, const Corner * c,const Corner * c1);
            float deltaAngle(Neighborhood n, const Corner * c);
            float angle(const Corner * c);




            map<Corner*, float> corner2tgs; //DEBUG
            map<Corner*, float> corner2dist;//DEBUG

        };
    map<Corner::Neighborhood, float> nbh2tgRef;

    vector<Corner> corners;
    vector<Corner*> findBotRights();
    vector<Corner*> getRow(Corner * start);


public:
    ChessBoardCornerSorter(const Size & size, const vector<Point2f> & corners);

    vector<vector<Point2f>> getSortedCornerPoints();
    vector<vector<Corner*>> getSortedCorners();
    vector<Point2f> getSortedCornerPoints1D();

};

#endif // CORNERSSORTER_H
