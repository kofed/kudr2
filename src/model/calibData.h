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

    //размер клетки шахматной доски мм
        int cellSize;
    //chess board with disparity
    ChessBoard leftPx;
    ChessBoard rightPx;
    ChessBoard leftSm;
    ChessBoard rightSm;

    Surface(){};

public:
    Surface(const int & _h,
		const ChessBoard & _leftPx,
		const ChessBoard & _rightPx,
		const ChessBoard & _leftSm,
		const ChessBoard & _rightSm)
    :h(_h),
	 leftPx(_leftPx),
	 rightPx(_rightPx),
	 leftSm(_leftSm),
	 rightSm(_rightSm){}

    Surface(const int _h,
            const int _cellSize,
            const ChessBoard & _leftPx,
            const ChessBoard & _rightPx)
        :h(_h),
          cellSize(_cellSize),
        leftPx(_leftPx),
        rightPx(_rightPx),
        leftSm(_leftPx.toSm()),
        rightSm(_rightPx.toSm()){
    }

	ChessBoard * getLeftPx(){return &leftPx;}
	ChessBoard * getRightPx(){return &rightPx;}
	ChessBoard * getLeftSm(){return &leftSm;}
	ChessBoard * getRightSm(){return &rightSm;}
    int getHeight(){return h;}

    void toYml(FileStorage & yml) const {
        //yml << "surface {:";
        yml << "h" << h;
        yml << "cellSize" << cellSize;
        yml << "leftPx";
        leftPx.toYml(yml);
        yml << "rightPx";
        rightPx.toYml(yml);
        yml << "leftSm";
        leftSm.toYml(yml);
        yml << "rightSm";
        rightSm.toYml(yml);
        //yml << "}";
    }

    static Surface fromYml(FileNode & fn){
        Surface surface;
    	fn["h"] >> surface.h;
        fn["cellSize"] >> surface.cellSize;
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
            yml << "{:";
            s.toYml(yml);
            yml << "}";
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
