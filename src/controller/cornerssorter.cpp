#include "cornerssorter.h"

vector<Point2f> CornerSorter::sort(){
    //int iTopLeft = findTopLeft();
    //swap(0, iTopLeft);

    for(int i = 0; i < corners.size()/2; ++i){
        int j = findNext(i);
    //    swap(i+1, j);
    }
}

int CornerSorter::findNext(const int i){
    return 0;
}

float CornerSorter::dist(const int i, const int j){
    return pow(corners[i].x - corners[j].x, 2.0f) + pow(corners[i].y - corners[j].y, 2.0f);
}
