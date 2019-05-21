#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <vector>
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;



class ChessBoard{

protected:
	//sorted corners
	vector<vector<Point2f>> corners;

	Size resolution;
    Point2f center;
    Point2i centerIndex;

    Size size;
    //sm
    float cellSize;


public:
    ChessBoard(const vector<vector<Point2f>> & _corners,  const Size _size,
               const Point2f _center, const Point2i _centerIndex, const float _cellSize, const Size _resolution);

    ChessBoard(const vector<Point2f> & corners, const Size _size,
               const Point2f _center, const Point2i _centerIndex, const float _cellSize, const Size _resolution);

    ChessBoard(){}

	const Point2f get(Point2i index) const;

	const Point2f get(const int x, const int y) const;
	
	const vector<vector<Point2f>> & getCorners() const {return corners;}

    /**
    * перестроить все индексы так, чтобы угол с  индексом index имел индекс refIndex
    * точки с отрицательными индексами будут отброшены
    * должно быть refIndex < index ибо не будем создавать пустые строки или столбцы
    */
    ChessBoard trim(Point2i index) const;

    ChessBoard toSm() const;

	void toYml(FileStorage & yml) const;

	static ChessBoard fromYml(cv::FileNode & fn);

    inline float getCellSize() const{return cellSize;}
    inline Size getSize() const{return size;}
    inline Point2f getCenter() const{return center;}
    inline Point2i getCenterIndex() const{return centerIndex;}
    inline Size getResolution() const {return resolution;}

    friend void operator>>(const FileNode & fn, ChessBoard & cb);
    friend FileStorage & operator<<(FileStorage & fn, const ChessBoard & cb);
};

//namespace cv{
//void operator>>(const FileNode & fn, ChessBoard & cb);
//}

#endif
