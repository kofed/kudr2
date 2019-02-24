#include "calibcontroller.h"
#include <iostream>

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
public:
    Vector2Iterator(vector<vector<Point2f>> & _v, Size _size):v(_v), size(_size) {}

    Point2f get(int xIndx, int yIndx){
        return v[0][xIndx*size.width + yIndx];
    }
};

CalibController::CalibController(const Controller & _controller, map<Position, Rect> & _rois)
    :controller(_controller), rois(_rois){

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

    drawChessboardCorners(image, size, Mat(corners[0]), patternWasFound);

    return corners;
}

void CalibController::findChessboardCorners(){
    for(auto p : positions){
        if(!images[p].data){
            throw runtime_error("Отсутствует файл с изображением шахматной доски");
        }

        corners[p] = findChessboardCorners(images[p], sizes[p]);

        if(corners[p][0].size() == 0){
            throw runtime_error("Не удалось найти углы. Возможно неверно указан размер доски");
        }
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

    for(auto p : positions){
        if(centers[p].x == 0 || centers[p].y == 0){
            throw runtime_error("Выделите центр на шахматной доске");
        }
    }

    CalibShot shot(h, cellSize);

    SpiralIterator itCentersL(findClosestCornerIndex(centers[LEFT], LEFT), sizes[LEFT]);
    SpiralIterator itCentersR(findClosestCornerIndex(centers[RIGHT], RIGHT), sizes[RIGHT]);

    Vector2Iterator itCornersL(corners[LEFT], sizes[LEFT]);
    Vector2Iterator itCornersR(corners[RIGHT], sizes[RIGHT]);

    while(itCentersL.next() | itCentersR.next()){
        shot.corners.push_back(
                    CalibCorner(itCornersL.get(itCentersL.getIX(), itCentersL.getIY()),
                itCornersR.get(itCentersR.getIX(), itCentersR.getIY()), centers[LEFT]));
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
//Возвращает индексы угла по координатам
Point2i CalibController::findClosestCornerIndex(const Point2i & point, const Position & pos){
    Point2i index(0, 0);
    int ix = 0, iy = 0;
    int d = 10000;

    Vector2Iterator it(corners[pos], sizes[pos]);
    //auto cornersPos = corners[pos];
    while(true){
        Point2i p = it.get(ix, iy);
        int _d = (point.x - p.x)*(point.x - p.x) + (point.y - p.y)*(point.y - p.y);
        if(_d < d){
            d = _d;
            index.x = ix;
            index.y = iy;
        }
        ++iy;
        if(iy == sizes[pos].height){
            iy = 0;
            ++ix;
        }
        if(ix == sizes[pos].width){
            break;
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
