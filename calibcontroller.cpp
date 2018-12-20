#include "calibcontroller.h"

CalibController::CalibController()
{

}

vector<vector<cv::Point2f>> & CalibController::findChessboardCorners(Position pos, Mat & image, const Size & size){
    if(size.height < 1 || size.width < 1){
        throw std::runtime_error("Размер доски меньше нуля\n");
    }
    vector<vector<cv::Point2f>> corners(1);
    bool patternWasFound =
            cv::findChessboardCorners( image, size, corners[0], CV_CALIB_CB_ADAPTIVE_THRESH );

    drawChessboardCorners(image, size, Mat(corners[0]), patternWasFound);
  //  lastCorners[pos] = corners;
    return corners;
}

void CalibController::deleteCorners(int pos){
    lastCorners.clear();
}

void CalibController::addCorners(const int distance){
    Position positions[] = {Position::LEFT, Position::RIGHT};
    for(auto p : positions){
        cache[p].push_back(lastCorners[p]);
    }
}

void CalibController::saveYML(){
    FileStorage fs("chessboard.yml", FileStorage::WRITE);
    Position positions[] = {Position::LEFT, Position::RIGHT};
    for(auto p : positions){
        for(auto entity : cache[p] ){
            entity.toYml(fs);
        }
    }
    fs.release();
}
