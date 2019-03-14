#include "calibcontroller.h"
#include <iostream>
#include "opencv2/calib3d.hpp"
#include "cornerssorter.h"

CalibCorner::CalibCorner(const Point2f & _pointL, const Point2f & _pointR, const Point2i & center)
    :pointL(_pointL), pointR(_pointR), p(pointL.x - center.x, pointL.y - center.y){

}

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
 */
class Vector2Iterator
{
    vector<vector<Point2f>> & v;

    Size & size;

    int xIndx = -1, yIndx = 0;

public:
    Vector2Iterator(vector<vector<Point2f>> & _v, Size _size):v(_v), size(_size) {}

    Point2f get(){
        return get(xIndx, yIndx);
    }

    Point2f get(int xIndx, int yIndx){
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

    int getIX(){return xIndx;}
    int getIY(){return yIndx;}
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
    vector<vector<cv::Point2f>> corners(1);
    bool patternWasFound =
            cv::findChessboardCorners( image, size, corners[0], CV_CALIB_CB_ADAPTIVE_THRESH );

        return corners;
}

void CalibController::findChessboardCorners(){
    for(auto p : positions){
        if(!images[p].data){
            throw runtime_error("Отсутствует файл с изображением шахматной доски");
        }

        corners[p] = findChessboardCorners(images[p], sizes[p]);

        if(corners[p][0].size() == 0){
            Logger::log("Не удалось найти углы. Возможно неверно указан размер доски");
        }

        if(!patternWasFound(p)){
            Logger::log("Не удалось найти все углы. Пожалуйста, исправьте результат с помощью кнопок Добавить/Удалить");
        }

        sortCorners(p);
    }
}

void CalibController::deleteCorners(){
    corners.clear();
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
        if(centers[p].x == 0 || centers[p].y == 0){
            throw runtime_error("Выделите центр на шахматной доске");
        }
    }

    CalibShot shot(h, cellSize);

    SpiralIterator itCentersL(findClosestCornerIndexSorted(centers[LEFT], LEFT), sizes[LEFT]);
    SpiralIterator itCentersR(findClosestCornerIndexSorted(centers[RIGHT], RIGHT), sizes[RIGHT]);

    Vector2Iterator itCornersL(corners[LEFT], sizes[LEFT]);
    Vector2Iterator itCornersR(corners[RIGHT], sizes[RIGHT]);

    while(itCentersL.next() | itCentersR.next()){
        shot.corners.push_back(
                    CalibCorner(itCornersL.get(itCentersL.getIX(),itCentersL.getIY()),
                itCornersR.get(itCentersR.getIX(),itCentersR.getIY()), centers[LEFT]));
    }
    corners.clear();
    cache.push_back(shot);
}

void CalibController::saveYML(){
    FileStorage fs("/tmp/chessboard.yml", FileStorage::WRITE);
    for(auto shot : cache ){
            shot.toYml(fs);
    }
    fs.release();
}

void CalibController::sendYML(){
    for(auto camera : controller.cameras){
        if(camera == NULL){
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
    float d = 100000.0f;
    vector<Point2f>::iterator closest = corners[pos][0].begin();
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
    Mat image = imread(path.toStdString());

    if(!image.data){
        throw runtime_error(QString("Не удалось открыть файл %1").arg(path).toStdString());
    }

    images[pos] = image;
}

Mat CalibController::getImageWithCorners(const Position pos){
    if(sizes[pos].area() == 0){
        throw runtime_error("Укажите размер доски");
    }

    if(corners[pos].size() != 0){
        Mat cornersImage;
        images[pos].copyTo(cornersImage);
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
    corners[pos][0].push_back(corner);
    sortCorners(pos);
}

void CalibController::sortCorners(const Position pos){
    vector<Point2f> & _corners = corners[pos][0];

    if(_corners.size() < sizes[pos].width * sizes[pos].height){
        Logger::log("Добавьте угол");
        return;
    }
    if(_corners.size() > sizes[pos].width * sizes[pos].height){
        Logger::log("Удалите лишние углы");
        return;
    }

    ChessBoard board(sizes[pos], _corners);
   // board.init(_corners);
    corners[pos][0] = board.getSortedCornerPoints1D();
    /*sort(_corners.begin(), _corners.end(),
        [](const Point2f & a, const Point2f & b) -> bool
    {
        float dy = a.y - b.y;
        if(abs(dy) < 20){
            float dx = a.x - b.x;
            return dx < 0;
        }

        return a.y - b.y < 0;
    });*/
}

void CalibController::deleteCorner(const Position pos, const Point2f click){
    auto it = findClosestCorner(click, pos);
    corners[pos][0].erase(it);
    sortCorners(pos);
}
