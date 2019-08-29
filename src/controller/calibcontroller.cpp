#include "calibcontroller.h"
#include <iostream>
#include "opencv2/calib3d.hpp"
#include "cornerssorter.h"

bool SpiralIterator::next(){
    double PI = 3.14159265;
    if(PI - phi < 0.001){
        phi = 0;
        ++step;
    }        
    double dPhi = PI/(4 * step);

    double dx = step * cos(phi);
    double dy = step * sin(phi);

    iX = centerIndex.x + round(dx);
    iY = centerIndex.y + round(dy);

    if(iX < 0 || iY < 0 || iX >= size.width || iY >= size.height)
	return false;
    
    cout << phi << " " << step << " " << iX << " " << iY << endl;

    phi += dPhi;

    return true;
}

int SpiralIterator::getIX(){
    return iX;
}

int SpiralIterator::getIY(){
    return iY;
}

/**
 * Class to iterate over CHess Board Find Corners result. Because there is a bug in findChessBoardResult
 * Резльтаты поиска углов cv возвращает как двумерный вектор v[i][j], причем все углы находятся в v[0]
 */
class Vector2Iterator
{
    const vector<vector<Point2f>> & v;

    const Size size;

    int xIndx = -1, yIndx = 0;

public:
    Vector2Iterator(const vector<vector<Point2f>> & _v, const Size _size):v(_v), size(_size) {}

    Point2f get() const{
        return get(xIndx, yIndx);
    }

    Point2f get(int xIndx, int yIndx) const {
        return v[0][xIndx+ size.width*yIndx];
    }

    bool next(){
        ++xIndx;
        if(xIndx == size.width){
            xIndx = 0;
            ++yIndx;
        }
        if(yIndx == size.height){
            return false;
        }

        return true;
    }

    int getIX() const {return xIndx;}
    int getIY() const {return yIndx;}
};

CalibController::CalibController(const Controller & _controller, map<Position, Rect> & _rois)
    :controller(_controller), rois(_rois){

    corners[LEFT].push_back(vector<Point2f>());
    corners[RIGHT].push_back(vector<Point2f>());
}

vector<vector<Point2f>> CalibController::findChessboardCorners(Mat & image, const Size & size){
    if(size.height < 1 || size.width < 1){
        throw std::runtime_error("Размер доски меньше нуля\n");
    }
    if(image.cols < 1 || image.rows < 1){
        throw runtime_error("изображение отсутствует.");
    }
    vector<vector<cv::Point2f>> _corners(1);
    cv::findChessboardCorners( image, size, _corners[0], CV_CALIB_CB_ADAPTIVE_THRESH );

        return _corners;
}

void CalibController::findChessboardCorners(){
    for(auto p : positions){
        if(!images[p].data){
            throw runtime_error("Отсутствует файл с изображением шахматной доски");
        }

        corners[p] = findChessboardCorners(images[p], sizes[p]);

        if(corners[p][0].size() == 0){
            Logger::log(p, "Алгоритм opencv не смог найти углы. Возможно неверно указан размер доски");
            continue;
        }

        if(patternWasFound(p) != 0){
            Logger::log("Не удалось найти все углы. Пожалуйста, исправьте результат с помощью кнопок Добавить/Удалить");
            continue;
        }

        sortCorners(p);
    }
}

void CalibController::addCalibEntities(const int h, const int cellSize){
    if(corners.size() <= 0){
        throw runtime_error("Произвидите поиск углов");
    }

    if(cellSize <= 0){
        throw runtime_error("Укажите размер доски");
    }

    if(h <= 0){
        throw runtime_error("Укажите высоту");
    }

    for(auto p : positions){
        if(centers[p].x < 10.0f || centers[p].y < 10.0f){
            throw runtime_error("Выделите центр на шахматной доске");
        }
    }

    Point2i centerL = findClosestCornerIndexSorted(centers[LEFT], LEFT);
    ChessBoard cbL(corners[LEFT][0], sizes[LEFT], centers[LEFT], centerL, cellSize, images[LEFT].size());

    Point2i centerR = findClosestCornerIndexSorted(centers[RIGHT], RIGHT);
    ChessBoard cbR(corners[RIGHT][0], sizes[RIGHT], centers[RIGHT], centerR, cellSize, images[RIGHT].size());

    cbR = cbR.trim(centerL, cbL.getSize() );
    cbL = cbL.trim(centerR, cbR.getSize() );

    Surface surface(h, cellSize, cbL, cbR);
    clear();
    cache.surfaces.push_back(surface);
}


void CalibController::saveYML(){
    FileStorage fs("/tmp/chessboard.json", FileStorage::WRITE);
    cache.toYml(fs);
    fs.release();
    cache = CalibData();
}

void CalibController::sendYML(){
    for(auto camera : controller.cameras){
        if(camera == nullptr){
            continue;
        }
        SShController sshController;
        try{
            sshController.init(camera->toString());
            sshController.fileTo("/tmp/chessboard.yml", "/tmp/chessboard.yml");
        }catch(std::exception & e){
            sshController.shutdown();
            throw e;
        }
    }
}

vector<Point2f>::iterator CalibController::findClosestCorner(const Point2f & point, const Position & pos){
    float d = MAXFLOAT;
    vector<Point2f>::iterator closest = corners[pos][0].end();
    for(vector<Point2f>::iterator it = corners[pos][0].begin(); it != corners[pos][0].end(); ++it){
        float _d = (point.x - it->x)*(point.x - it->x) + (point.y - it->y)*(point.y - it->y);
        if(_d < d){
            d = _d;
            closest = it;
        }
    }
    return closest;
}
//Возвращает индексы угла по координатам
Point2i CalibController::findClosestCornerIndexSorted(const Point2f & point, const Position & pos){
    Point2i index(0, 0);
    float d = 10000.0f;

    Vector2Iterator it(corners[pos], sizes[pos]);
    //auto cornersPos = corners[pos];
    while(it.next()){
        Point2i p = it.get();
        float _d = (point.x - p.x)*(point.x - p.x) + (point.y - p.y)*(point.y - p.y);
        if(_d < d){
            d = _d;
            index.x = it.getIX();
            index.y = it.getIY();
        }

    }
    return index;
}

void CalibController::openImage(const Position pos, const QString &path){
    clear();

    images[pos] = imread(path.toStdString());

    if(!images[pos].data){
        throw runtime_error(QString("Не удалось открыть файл %1").arg(path).toStdString());
    }

}

void CalibController::clear(){


    corners[LEFT][0].clear();
    corners[RIGHT][0].clear();


    centers.clear();
    rois.clear();

    sizes.clear();
}

Mat CalibController::getImageWithCorners(const Position pos){
    if(corners[pos].size() != 0){
        Mat cornersImage;
        images[pos].copyTo(cornersImage);
       // vector<Point2f> _corners;
      //  _corners.insert(_corners.begin(), corners[pos][0].begin(), corners[pos][0].end());
        drawChessboardCorners(cornersImage, sizes[pos], Mat(corners[pos][0]), true);
        return cornersImage;
    }else{
        return images[pos];
    }

}

int CalibController::patternWasFound(const Position pos){
    return (int)corners[pos][0].size() - sizes[pos].area();
}

void CalibController::addCorner(const Position pos, const Point2f corner){
    if(sizes[pos].area() == 0){
        throw runtime_error("Укажите размер доски");
    }

    corners[pos][0].push_back(corner);
    sortCorners(pos);
}

void CalibController::sortCorners(const Position pos){
    vector<Point2f> & _corners = corners[pos][0];

    int diff = patternWasFound(pos);

    if(diff < 0){
        Logger::log("Добавьте угол" + QString::number(diff));
        return;
    }else
    if(diff > 0){
        Logger::log("Удалите лишние углы" + QString::number(diff));
        return;
    }

    ChessBoardCornerSorter board(sizes[pos], _corners);
   // board.init(_corners);
//    corners[pos][0] = /*board.getSortedCornerPoints1D();*/_corners;

}

void CalibController::deleteCorner(const Position pos, const Point2f click){
    auto it = findClosestCorner(click, pos);
    if(it == corners[pos][0].end()){
        throw runtime_error("Нет угла для удаления");
    }
    corners[pos][0].erase(it);
    sortCorners(pos);
}
