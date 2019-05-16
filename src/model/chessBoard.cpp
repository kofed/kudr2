#include "chessBoard.h"

ChessBoard::ChessBoard(const vector<vector<Point2f>> & _corners,
                       const Point2f _center, const Point2i _centerIndex)
    :corners(_corners), center(_center), centerIndex(_centerIndex){

}

ChessBoard::ChessBoard(const vector<Point2f> & _corners, const Size _size):size(_size){
    if(_corners.size() != _size.area()){
        throw runtime_error("Неверно указан размер шахматной доски");
    }

    corners.resize((unsigned)size.height);
    for(auto row : corners){
        row.resize((unsigned)size.width);
    }

    auto pt = _corners.begin();
    for(unsigned i = 0; i < size.height; ++i){
        for(unsigned j = 0; j < size.width; ++j){
            corners[i].assign(j, *pt++);
        }
    }
}



const Point2f ChessBoard::get(const Point2i index) const{
	return corners[index.x][index.y];
}

const Point2f ChessBoard::get(const int x, const int y) const{
	return corners[x][y];
}

ChessBoard ChessBoard::trim(Point2i index) const{
    vector<vector<Point2f>> _corners;
    unsigned _sizeY = corners.size() - abs(index.y);

    unsigned dXBegin = 0;
    unsigned dXEnd = 0;
    unsigned dY = 0;


    if(index.x > 0){
        dXBegin = index.x;
    }else{
        dXEnd = -index.x;
    }

    if(index.y < 0){
        dY = -index.y;
    }

    _corners.resize(_sizeY);
    for(unsigned i = 0; i < _sizeY; ++i){
        _corners[i].insert(_corners[i].begin(),
                corners[i + dY].begin() + dXBegin,
                corners[i + dY].end() - dXEnd);
    }

    Point2i _centerIndex(centerIndex.x - dXBegin,
                         index.y > 0 ? centerIndex.y - index.y: centerIndex.y );


    return ChessBoard(_corners, center,  _centerIndex);
}

ChessBoard ChessBoard::toSm() const{
    vector<vector<Point2f>> _corners;
    _corners.resize(corners.size());
    for(unsigned i = 0; i < corners.size(); ++i){
        _corners[i].resize(corners[i].size());
        for(unsigned j = 0; j < corners[i].size(); ++j ){
            _corners[i][j] = Point2f(center.x + (i - centerIndex.x) * cellSize,
                        center.y + (j - centerIndex.y) * cellSize);
        }

    }
    return ChessBoard(_corners);
}

void ChessBoard::toYml(FileStorage & yml) const{
    yml << "{";
    yml << "corners" << "[:";
    for(auto row : corners){
        for(auto c : row){
          // yml << "{:";
           yml << c;
        //   yml << "}";
        }
     }
    yml << "]";
    yml << "}";
}


//namespace cv{
void operator>>(const FileNode & fn, ChessBoard & cb){
        vector<Point2f> corners;
        FileNode fnCorners = fn["corners"];
        for(FileNodeIterator it = fnCorners.begin() ; it != fnCorners.end(); ++it){
            FileNode fn = *it;
            Point2f p;
            fn >> p;
            corners.push_back(p);
        }
        cb = ChessBoard(corners, Size(1,1));

	}
//}
