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

    //sm
	float cellSize;

    Point2f center;
    Point2i centerIndex;

    Size size;
public:
    ChessBoard(const vector<vector<Point2f>> & _corners,
               const Point2f _center, const Point2i _centerIndex);

    ChessBoard(const vector<Point2f> & corners, const Size _size, const Point2f _center, const Point2i _centerIndex);

	ChessBoard(){};

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

    inline float getCellSize(){return cellSize;}

    friend void operator>>(const FileNode & fn, ChessBoard & cb);
};

//namespace cv{
//void operator>>(const FileNode & fn, ChessBoard & cb);
//}

#endif
