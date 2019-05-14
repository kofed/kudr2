#include "chessBoard.h"

ChessBoard::ChessBoard(const vector<vector<Point2f>> & _corners):corners(_corners){

}

const Point2f ChessBoard::get(const Point2i index) const{
	return corners[index.x][index.y];
}

const Point2f ChessBoard::get(const int x, const int y) const{
	return corners[x][y];
}


namespace cv{
void operator>>(const FileNode & fn, ChessBoard & cb){
		//chessBoard = fromYml(fn);

	}
}
