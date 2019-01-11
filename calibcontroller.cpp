#include "calibcontroller.h"

bool ChessBoardCenterIterator::next(){
    if(++phi == step * 8){
        ++step;
        phi = 0;
    }

    int d = phi / step;

    if(d >= 0 && d <= 2){
        iX = center.x + step;
    }else if(d >= 4 && d <= 6){
        iX = center.x - step;
    }

    if(d >= 2 && d <= 4){
        iY = center.y + step;
    }else if(d >= 6 && d <= 8){
        iY = center.y - step;
    }

    if(iX < 0 || iY < 0){
        return false;
    }
    return true;
}

int ChessBoardCenterIterator::getIX(){
    return iX;
}

int ChessBoardCenterIterator::getIY(){
    return iY;
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
  //  lastCorners[pos] = corners;

    return corners;
}

void CalibController::findChessboardCorners(const Size & sizeL, const Size & sizeR){
    string fnameL = controller.getImgFileName(LEFT).toStdString();
    string fnameR = controller.getImgFileName(RIGHT).toStdString();

    Mat imageL = imread(fnameL);
    Mat imageR = imread(fnameR);
    if(!imageL.data || !imageR.data){
        throw runtime_error("Отсутствует файл с изображением шахматной доски");
    }

    corners[LEFT] = findChessboardCorners(imageL, sizeL);

    corners[RIGHT] = findChessboardCorners(imageR, sizeR);

    imwrite(fnameL, imageL);
    imwrite(fnameR, imageR);

}

void CalibController::deleteCorners(){
   /* corners[LEFT] = vector<Point2f>(0);
    corners[RIGHT] = vector<Point2f>(0);*/
}

void CalibController::addCalibEntities(const Point2i & lCenterIndex, const Point2i & rCenterIndex, const int h){
    ChessBoardCenterIterator itL(lCenterIndex);
    ChessBoardCenterIterator itR(rCenterIndex);
    while(itL.next() || itR.next()){
        cache.push_back(
                    CalibEntity(h,
                                corners[LEFT][itL.getIX()][itL.getIY()],
                corners[RIGHT][itR.getIX()][itR.getIY()]));
    }

}

void CalibController::saveYML(){
    FileStorage fs("chessboard.yml", FileStorage::WRITE);
    for(auto entity : cache ){
            entity.toYml(fs);
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
            sshController.fileTo("chessboard.yml", "/tmp/chessboard.yml");
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

    auto cornersPos = corners[pos];
    while(true){
        Point2i p = cornersPos[ix][iy];
        int _d = (point.x - p.x)^2 + (point.y - p.y)^2;
        if(_d < d){
            d = _d;
            index.x = ix;
            index.y = iy;
        }
        ++iy;
        if(iy == cornersPos[ix].size()){
            iy = 0;
            ++ix;
        }
        if(ix == cornersPos.size()){
            break;
        }
    }
    return index;
}

