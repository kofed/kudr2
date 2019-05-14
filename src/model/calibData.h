#ifndef CALIB_DATA_H
#define CALIB_DATA_H

#include "opencv2/opencv.hpp"
#include <utility>
#include <vector>
#include "chessBoard.h"

using namespace cv;
using namespace std;

class Surface{
private:
    //расстояние между плоскостями введенное оператором
    int h;
    //координаты центра. Введены оператором
    Point2f centerL, centerR;

    //chess board with disparity
    ChessBoard leftPx;
    ChessBoard rightPx;
    ChessBoard leftSm;
    ChessBoard rightSm;

    //размер клетки шахматной доски мм
        int cellSize;
    Surface(){};

public:
    Surface(const int & _h,
		const Point2f & _centerL,
		const Point2f & _centerR,
		const ChessBoard & _leftPx,
		const ChessBoard & _rightPx,
		const ChessBoard & _leftSm,
		const ChessBoard & _rightSm)
    :h(_h),
     centerL(_centerL),
	 centerR(_centerR),
	 leftPx(_leftPx),
	 rightPx(_rightPx),
	 leftSm(_leftSm),
	 rightSm(_rightSm){

}
	ChessBoard * getLeftPx(){return &leftPx;}
	ChessBoard * getRightPx(){return &rightPx;}
	ChessBoard * getLeftSm(){return &leftSm;}
	ChessBoard * getRightSm(){return &rightSm;}
	const int getHeight(){return h;}

    void toYml(FileStorage & yml) const {
    	yml << "h" << h;
    	yml << "centerL" << centerL;
    	yml << "centerR" << centerR;
    	leftPx.toYml(yml);
    	rightPx.toYml(yml);
    	leftSm.toYml(yml);
    	rightSm.toYml(yml);
    }

    static Surface fromYml(FileNode & fn){
        Surface surface;
    	fn["h"] >> surface.h;
    	fn["cellSize"] >> surface.cellSize;
        fn["centerL"] >> surface.centerL;
        fn["centerR"] >> surface.centerR;
        fn["leftPx"] >> surface.leftPx;
        fn["rightPx"] >> surface.rightPx;
        fn["leftSm"] >> surface.leftSm;
        fn["rightSm"] >> surface.rightSm;

        return surface;
    }
};

//модель для сохранения в yml
class CalibData{
public:

    //высота между плоскостями к углам
    vector<Surface> surfaces;

    CalibData(){

    }

    void toYml(FileStorage & yml) const{
        yml << "surfaces" << "[";
        for(auto s : surfaces){
            s.toYml(yml);
        }
        yml << "]";
    }

    static CalibData fromYml(const string & name){
        FileStorage yml(name, FileStorage::READ);
    	if(!yml.isOpened()){
    		throw runtime_error("не могу загрузить калибровочные данные. Файл не открыт");
    	}
        CalibData cs;

    	FileNode fnCorners = yml["surfaces"];
    	for(FileNodeIterator it = fnCorners.begin() ; it != fnCorners.end(); ++it){
            FileNode fn = *it;
    		cs.surfaces.push_back(Surface::fromYml(fn));
    	}
    	return cs;
    }
};

#endif
