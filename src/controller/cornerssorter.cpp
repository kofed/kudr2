#include "cornerssorter.h"
#include "logger.h"
#include <set>

ChessBoard::ChessBoard(const Size & _size, const vector<Point2f> & points):size(_size){
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

vector<vector<Point2f>> ChessBoard::getSortedCornerPoints(){
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
    vector<Point2f> unsorted;
    for(auto itUnsorted = cornersSet.begin(); itUnsorted != cornersSet.end(); ++itUnsorted){
        unsorted.push_back((*itUnsorted)->point);
    }
    
    points.push_back(unsorted);
    return points;
}

vector<vector<ChessBoard::Corner*>> ChessBoard::getSortedCorners(){
    vector<vector<Corner*>> sortedCorners;
    Corner * botRight = findTopLeft();
    Corner * rowStart = botRight;
    
    for(int i = 0; i < size.height; ++i){
        vector<Corner*> row = getRow(rowStart);
        sortedCorners.push_back(row);
        Corner* rowStartPrev = rowStart;
        rowStart = rowStart->getNeighbor(Corner::UP);
        if(rowStart == 0x0 && i != size.height - 1){
            Logger::log("Отсутствует угол снизу. Скорректируйте положение углов");
            break;
        }
    }
    if(rowStart != NULL){
        Logger::log("Столбец содержит больше строк чем ожидается. Скорректируйте положение углов");
    }       

    return sortedCorners;
}

vector<Point2f> ChessBoard::getSortedCornerPoints1D(){
    vector<vector<Point2f>> points2D = getSortedCornerPoints();
    vector<Point2f> points1D;
    for(auto i = 0; i < points2D.size(); ++i){
         points1D.insert(points1D.end(), points2D[i].begin(), points2D[i].end());
    }
    return points1D;
}

vector<ChessBoard::Corner*> ChessBoard::getRow(Corner * start){
    vector<Corner*> row;
    Corner * corner = start;


    for(int i = 0; i < size.width; ++i){
        row.push_back(corner);
        Corner* prev = corner;
        corner = corner->getNeighbor(Corner::LEFT);
        if(corner == 0x0 && i != size.width - 1){
            Logger::log("Отсутствует угол справа. Скорректируйте положение углов");
            break;

        }
    }
    if(corner != NULL){
        Logger::log("Конечный угол строки имеет соседа. В строке больше элементов чем размер. Скорректируйте положение углов");
    }
    return row;
}

ChessBoard::Corner::Corner(const Point2f & _point):point(_point){

}

ChessBoard::Corner* ChessBoard::findTopLeft(){
    return &*find_if(corners.begin(), corners.end(), [](Corner & c){
        return c.getNeighbor(Corner::LEFT) != NULL &&
                c.getNeighbor(Corner::RIGHT) == NULL &&
                c.getNeighbor(Corner::BOT) == NULL &&
                c.getNeighbor(Corner::UP) != NULL;
    });
}

float ChessBoard::Corner::dist(const Corner *c){
    return sqrt(pow(c->point.x - point.x, 2.0f)
                + pow(c->point.y - point.y, 2.0f));
}

ChessBoard::Corner::Neighborhood ChessBoard::Corner::findNeighborhood(const Corner* c){
    float dx = c->point.x - point.x;
    float dy = c->point.y - point.y;
    float tg = dy/dx;

    //DEBUG
    corner2tgs[const_cast<Corner*> (c)] = tg;

    const float TG_45_MAX = 1.8f;
    const float TG_45_MIN = 0.68f;
    const float TG_90_MAX = 0.48f;
    const float TG_0 = 3.0f;

    if(dy >= 0){
        if(tg < -TG_0 || tg > TG_0){
            return UP;
        }else if(tg > TG_45_MIN && tg < TG_45_MAX){
            return UPRIGHT;
        }else if(tg > -TG_45_MAX && tg < -TG_45_MIN){
            return UPLEFT;
        }else if(tg > -TG_90_MAX && tg < TG_90_MAX){
            if(dx > 0){
                return LEFT;
            }else{
                return RIGHT;
            }

        }
    }else{
        if(tg < -TG_0 || tg > TG_0){
            return BOT;
        }else if(tg > TG_45_MIN && tg < TG_45_MAX){
            return BOTLEFT;
        }else if(tg > -TG_45_MAX && tg < -TG_45_MIN){
            return BOTRIGHT;
        }else if(tg > -TG_90_MAX && tg < TG_90_MAX){
            if(dx > 0){
                return LEFT;
            }else{
                return RIGHT;
            }
        }
    }
    return BOTRIGHT;
}

void ChessBoard::Corner::findAndSetNeighborhood(
        const vector<Corner>::iterator neibBegin,
        const vector<Corner>::iterator  neibEnd){

    vector<float> distancis;
    map<float, Corner*> distToCorner;
    for(auto it = neibBegin; it != neibEnd; ++it){
        if(&*it == this){
            continue;
        }
        float d = dist(&*it);
        distToCorner[d] = &*it;
        distancis.push_back(d);
    }
    sort(distancis.begin(), distancis.end(),
         [](const float & f1, const float & f2){
        return f1 < f2;
    });
    if(distancis.size() < 3){
        throw  runtime_error("Ошибка алгоритма. У угла соседей меньше трех");
    }else  if(distancis.size() >8){
            distancis.erase(distancis.begin() + 8, distancis.end());
    }

    float maxDist = distancis[0] * 2.5f;
    for(auto i = 0; i < distancis.size(); ++i){
        if(distancis[i] > maxDist){
            if(i < 3){
              //  throw runtime_error("Число соседей меньше трех. Остальные углы находятся далее чем d max");
            }
            break;
        }

        auto neib = distToCorner[distancis[i]];
        Neighborhood n = findNeighborhood(neib);

        setNeighbor(n, neib);

    }

}

bool ChessBoard::Corner::checkNeighborhood(const Neighborhood n, const Corner* corner){
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

void ChessBoard::Corner::setNeighbor(const Neighborhood n, Corner* corner){
    if(neighbors.find(n) != neighbors.end()){
        return;
    }

    if(!checkNeighborhood(n, corner)){
        return;
    }

    neighbors[n] = corner;
    distances[n] = dist(corner);
}

ChessBoard::Corner* ChessBoard::Corner::getNeighbor(const Neighborhood n)  {
    return neighbors[n];
}
