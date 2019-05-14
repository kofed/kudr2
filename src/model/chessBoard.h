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

	float cellSize;

public:
	ChessBoard(const vector<vector<Point2f>> & corners);

	ChessBoard(){};

	const Point2f get(Point2i index) const;

	const Point2f get(const int x, const int y) const;
	
	const vector<vector<Point2f>> & getCorners() const {return corners;}

	const Size size;

	void toYml(FileStorage & yml) const;

	static ChessBoard fromYml(cv::FileNode & fn);

	inline float getCellSize(){return cellSize;};
};

namespace cv{
void operator>>(const FileNode & fn, ChessBoard & cb);
}

#endif
