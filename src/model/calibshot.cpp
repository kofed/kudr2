#include "calibshot.h"

Surface::Surface(const int & _h, const Point2f & _centerL, const Point2f & _centerR, const map<Point2f, Point2f> _corners)
    :centerL(_centerL), centerR(_centerR), corners(_corners), h(_h){

}
