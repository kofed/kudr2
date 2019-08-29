#include "cornerssorter.h"
#include "logger.h"
#include <set>

ChessBoardCornerSorter::ChessBoardCornerSorter(const Size & _size, const vector<Point2f> & points):size(_size){
    if(points.size() < 9 || size.width < 3 || size.height <3){
        throw runtime_error("Доска слишком маленькая.");
    }

    for(auto p :points){
        corners.push_back(Corner(p));
    }

    for(auto it = corners.begin(); it != corners.end(); ++it){
        it->findAndSetNeighborhood(corners.begin(), corners.end());
    }
}

vector<vector<Point2f>> ChessBoardCornerSorter::getSortedCornerPoints(){
    list<Corner*> cornersSet;
    for (auto it = corners.begin(); it != corners.end(); ++it) {
        cornersSet.push_back(&*it);
    }

    vector<vector<Corner*>> sortedCorners = getSortedCorners();
    vector<vector<Point2f>> points;
    for (vector<vector<Corner*>>::iterator itRow = sortedCorners.begin(); itRow != sortedCorners.end(); ++itRow) {
        vector<Point2f> row;
        for(vector<Corner*>::iterator it = itRow->begin(); it != itRow->end(); ++it){
            row.push_back((*it)->point);
            cornersSet.remove(*it);
        }

        points.push_back(row);
    }
    //vector<Point2f> unsorted;
    //for(auto itUnsorted = cornersSet.begin(); itUnsorted != cornersSet.end(); ++itUnsorted){
    //    unsorted.push_back((*itUnsorted)->point);
    //}
    
    //points.push_back(unsorted);
    return points;
}

vector<vector<ChessBoardCornerSorter::Corner*>> ChessBoardCornerSorter::getSortedCorners(){
    vector<vector<Corner*>> sortedCorners;

    vector<Corner*> botRights = findBotRights();

    int i = 0;
    for(Corner* rowStart : botRights){
        for(; i < size.height; ++i){
            vector<Corner*> row = getRow(rowStart);
            sortedCorners.push_back(row);
            rowStart = const_cast<Corner*>(rowStart->getNeighbor(Corner::UP));
            if(rowStart == nullptr && i != size.height - 1){
                Logger::log("Отсутствует угол сверху. Скорректируйте положение углов");
                break;
            }
        }
        if(rowStart != nullptr){
            Logger::log("Столбец содержит больше строк чем ожидается. Скорректируйте положение углов");
        }

    }

    return sortedCorners;
}

vector<Point2f> ChessBoardCornerSorter::getSortedCornerPoints1D(){
    vector<vector<Point2f>> points2D = getSortedCornerPoints();
    vector<Point2f> points1D;
    for(unsigned i = 0; i < points2D.size(); ++i){
         points1D.insert(points1D.end(), points2D[i].begin(), points2D[i].end());
    }
    return points1D;
}

vector<ChessBoardCornerSorter::Corner*> ChessBoardCornerSorter::getRow(Corner * start){
    vector<Corner*> row;
    Corner * corner = start;

    for(int i = 0; i < size.width; ++i){
        row.push_back(corner);
        corner = const_cast<Corner*> (corner->getNeighbor(Corner::LEFT));
        if(corner == nullptr && i != size.width - 1){
            Logger::log("Отсутствует угол слева. Скорректируйте положение углов");
            break;

        }
    }
    if(corner != nullptr){
        Logger::log("Конечный угол строки имеет соседа. В строке больше элементов чем размер. Скорректируйте положение углов");
    }
    return row;
}

ChessBoardCornerSorter::Corner::Corner(const Point2f & _point):point(_point){

}

vector<ChessBoardCornerSorter::Corner*> ChessBoardCornerSorter::findBotRights(){
    vector<Corner*> botRights;

    for(unsigned i = 0; i != corners.size(); ++i){
        if( corners[i].getNeighbor(Corner::LEFT) != nullptr &&
                corners[i].getNeighbor(Corner::RIGHT) == nullptr &&
                corners[i].getNeighbor(Corner::BOT) == nullptr &&
                corners[i].getNeighbor(Corner::UP) != nullptr){
            botRights.push_back(&corners[0] + i);
        }
    }
    sort(botRights.begin(), botRights.end(), ChessBoardCornerSorter::Corner::cmp);

    return botRights;
}

float ChessBoardCornerSorter::Corner::dist(const Corner *c){
    return sqrt(pow(c->point.x - point.x, 2.0f)
                + pow(c->point.y - point.y, 2.0f));
}

void ChessBoardCornerSorter::Corner::compareAndSet(Neighborhood n, const Corner* c){
    float t = tang(c);

    if(neighbors[n] == nullptr
            || /*tgDelta(n, nbh2tg[n] , t)*/ hashDelta(n, neighbors[n], c) > 0){
        nbh2tg[n] = t;
        neighbors[n] = c;
    };
}

float ChessBoardCornerSorter::Corner::hashDelta(Neighborhood n, const Corner * c,const Corner * c1){
    return 2*(abs(deltaAngle(n, c)) - abs(deltaAngle(n, c1))) / PI + (1 - pow(dist(c1), 2.0f) / pow(dist(c), 2.0f));
}

float ChessBoardCornerSorter::Corner::tang(const Corner * c){
    float dy = c->point.y - point.y;
    float dx = c->point.x - point.x;
    return dy / dx;
}

float ChessBoardCornerSorter::Corner::deltaAngle(Neighborhood n, const Corner * c){
    switch (n) {
        case LEFT:
            return PI/2 - angle(c);
        case RIGHT:
            return 3*PI/2 - angle(c);
        case UP:
            return PI -angle(c);
        case BOT:
            return angle(c);
    }
}

float ChessBoardCornerSorter::Corner::angle(const Corner * c){
    float dy = c->point.y - point.y;
    float dx = c->point.x - point.x;
    float tg = dy / dx;
    if(dx > 0){
        return PI/2 + atan(tg);
    }else{
        return 3*PI/2 + atan(tg);
    }
}

void ChessBoardCornerSorter::Corner::trySetNeighborhood(const Corner* c){

    float dy = c->point.y - point.y;
    float dx = c->point.x - point.x;
    float tg = dy / dx;

    //DEBUG
    corner2tgs[const_cast<Corner*> (c)] = tg;

    const float TG_45_MAX = 2.2f;
    const float TG_45_MIN = 0.65f;
    const float TG_90_MAX = 1.0f;
    const float TG_0 = 1.0f;

    if(dy >= 0){
        if((tg < -TG_0 || tg > TG_0)){
            compareAndSet(UP, c);
        }else if(tg > -TG_90_MAX && tg < TG_90_MAX){
            if(dx > 0){
                compareAndSet(LEFT, c);
            }else{
                compareAndSet(RIGHT, c);
            }

        }
    }else{
        if(tg < -TG_0 || tg > TG_0){
            return compareAndSet(BOT, c);
        }else if(tg > -TG_90_MAX && tg < TG_90_MAX){
            if(dx > 0){
                return compareAndSet(LEFT, c);
            }else{
                return compareAndSet(RIGHT, c);
            }
        }
    }
}

float ChessBoardCornerSorter::Corner::tgDelta(Neighborhood n, const Corner * c, const Corner * c1){
    switch (n) {
    case UP:
    case BOT:
        return abs(tang(c1)) - tang(c);
    case LEFT:
    case RIGHT:
        return abs(tang(c)) - abs(tang(c1));
    }
}

void ChessBoardCornerSorter::Corner::findAndSetNeighborhood(
        const vector<Corner>::iterator neibBegin,
        const vector<Corner>::iterator  neibEnd){

    map<float, Corner*> distToCorner;
    for(auto it = neibBegin; it != neibEnd; ++it){
        if(&*it == this){
            continue;
        }
        float d = dist(&*it);
        distToCorner[d] = &*it;
    }
    map<float, Corner*> tg2corner;
    auto it = distToCorner.begin();
    for(int i = 0; i < 8; ++i, ++it){
        trySetNeighborhood(it->second);
    }
}
/*
void ChessBoardCornerSorter::Corner::setNeighborhood(map<float, Corner*> & tg2corner){

}*/
/*
bool ChessBoardCornerSorter::Corner::checkNeighborhood(const Neighborhood n, const Corner* corner){
    if(n == LEFT || n == RIGHT || n == BOT || n == UP){
        for(auto neibIt = neighbors.begin(); neibIt != neighbors.end(); ++neibIt){
            float ratio = dist(corner)/distances[neibIt->first];
            if(neibIt->first == n){
                throw runtime_error("Скорректируйте доску, такой угол уже содержится");
            }
            if(neibIt->first == LEFT || neibIt->first == RIGHT || neibIt->first == BOT || neibIt->first == UP){
                if(ratio < 0.7f || ratio > 1.2f){
     //               return false;
                }
            }else{
                if(ratio < 1.2f || ratio > 1.7f){
    //                return false;
                }
            }
        }
    }else{
        for(auto neibIt = neighbors.begin(); neibIt != neighbors.end(); ++neibIt){
            float ratio = dist(corner)/distances[neibIt->first];
            if(neibIt->first == n){
                throw runtime_error("Скорректируйте доску, такой угол уже содержится");
            }
            if(neibIt->first == LEFT || neibIt->first == RIGHT || neibIt->first == BOT || neibIt->first == UP){
                if(ratio < 0.7f || ratio > 0.9f){
      //              return false;
                }
            }else{
                if(ratio < 0.7f || ratio > 1.2f){
        //            return false;
                }
            }
        }
    }
    return true;
}
*//*
void ChessBoardCornerSorter::Corner::setNeighbor(const Neighborhood n, Corner* corner){
    if(neighbors.find(n) != neighbors.end()){
        return;
    }

    if(!checkNeighborhood(n, corner)){
        return;
    }

    neighbors[n] = corner;
    distances[n] = dist(corner);
}*/

const ChessBoardCornerSorter::Corner* ChessBoardCornerSorter::Corner::getNeighbor(const Neighborhood n)  {
    return neighbors[n];
}
