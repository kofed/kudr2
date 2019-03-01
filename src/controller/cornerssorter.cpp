#include "cornerssorter.h"

ChessBoard::ChessBoard(vector<Point2f> points, const Size & _size):size(_size){
    if(points.size() < 9 || size.width < 3 || size.height <3){
        throw runtime_error("Доска слишком маленькая.");
    }

    for(auto p :points){
        corners.push_back(Corner(p));
    }

    for(auto it = corners.end(); it != corners.begin(); --it){


    }
}

float ChessBoard::dist(const Point2f & p1, const Point2f & p2){
    return pow(p1.x - p2.x, 2.0f) + pow(p1.y - p2.y, 2.0f);
}

void ChessBoard::Corner::findAndSetNeighborhood(
        vector<Corner>::const_iterator & neibBegin,
        vector<Corner>::const_iterator &  neibEnd){
    vector<float> distancis;
    for(auto it = neibBegin; it != neibEnd; ++it){
        distancis.push_back(dist(*it));
    }
    sort(distancis.begin(), distancis.end(),
         [](const float & f1, const float & f2){
        return f1 < f2;
    });
}

vector
