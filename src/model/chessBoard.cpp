#include "chessBoard.h"

ChessBoard::ChessBoard(const vector<vector<Point2f>> & _corners,  const Size _size,
                       const Point2f _center, const Point2i _centerIndex,
                       float _cellSize, const Size _resolution)
    :corners(_corners), center(_center), centerIndex(_centerIndex),
    cellSize(_cellSize), resolution(_resolution){
    size.height = _corners.size();
    size.width = _corners[0].size();

}

ChessBoard::ChessBoard(const vector<Point2f> & _corners, const Size _size,
                       const Point2f _center, const Point2i _centerIndex,
                        const float _cellSize, const Size _resolution)
                : center(_center), centerIndex(_centerIndex), size(_size), cellSize(_cellSize), resolution(_resolution){
    if(_corners.size() != _size.area()){
        throw runtime_error("Неверно указан размер шахматной доски");
    }

    corners.resize((unsigned)size.height);
  //  for(auto row : corners){
  //      row.resize((unsigned)size.width);
  //  }

    auto pt = _corners.begin();
    for(unsigned i = 0; i < size.height; ++i){
        for(unsigned j = 0; j < size.width; ++j){
            corners[i].push_back(*pt++);
        }
    }
}



const Point2f ChessBoard::get(const Point2i index) const{
    return corners[index.y][index.x];
}

const Point2f ChessBoard::get(const int x, const int y) const{
    return corners[y][x];
}

ChessBoard ChessBoard::trim(Point2i index) const{
    vector<vector<Point2f>> _corners;

    unsigned dXBegin = 0;
    unsigned dXEnd = 0;
    unsigned dYBegin = 0;
    unsigned dYEnd = 0;


    if(index.x > 0){
        dXBegin = index.x;
    }else{
        dXEnd = -index.x;
    }

    if(index.y > 0){
        dYBegin = index.y;
    }else{
        dYEnd = -index.y;
    }

    _corners.resize(corners.size() - abs(index.y));
    for(unsigned i = 0; i < _corners.size(); ++i){
        _corners[i].insert(_corners[i].begin(),
                corners[i + dYBegin].begin() + dXBegin,
                corners[i + dYBegin].end() - dXEnd);
    }

    Point2i _centerIndex(centerIndex.x - dXBegin, centerIndex.y - dYBegin);


    return ChessBoard(_corners, size,  center,  _centerIndex, cellSize, resolution);
}

ChessBoard ChessBoard::trim(const Point2i _centerIndex, const Size _size) const{
    Point2i dCenter =  centerIndex - _centerIndex;
    ChessBoard cb = trim(Point2i(dCenter.x > 0 ? dCenter.x: 0,
         dCenter.y > 0 ? dCenter.y: 0));

    Size dSize = size - _size;
    cb = cb.trim(Point2i(dSize.width > 0? -dSize.width: 0, dSize.height > 0? -dSize.height: 0));
    return cb;
}

ChessBoard ChessBoard::toSm() const{
    vector<vector<Point2f>> _corners;
    _corners.resize(corners.size());
    for(int i = 0; i < corners.size(); ++i){
        for(int j = 0; j < corners[i].size(); ++j ){
            float x = center.x + (j - centerIndex.x) * cellSize;
            float y = center.y + (i - centerIndex.y) * cellSize;
            Point2f p(x, y);
            _corners[i].push_back( p );
        }

    }
    return ChessBoard(_corners, size, center, centerIndex, cellSize, resolution);
}

void ChessBoard::toYml(FileStorage & yml) const{

}


void operator>>(const FileNode & fn, ChessBoard & cb){
        vector<vector<Point2f>> corners;
        fn["corners"] >> corners;

        Point2f center;
        Point2i centerIndex;
        Size size;
        Size resolution;
        fn["center"] >> center;
        fn["centerIndex"] >> centerIndex;
        fn["size"] >> size;
        float cellSize;
        fn["cellSize"] >> cellSize;
        fn["resolution"] >> resolution;
        cb = ChessBoard(corners, size, center, centerIndex, cellSize, resolution);

	}

FileStorage & operator<<( FileStorage & fs, const ChessBoard & cb){
   fs <<  "{";

    fs << "corners" << cb.corners;
    fs << "center" << cb.center;
    fs << "centerIndex" << cb.centerIndex;
    fs << "size" << cb.size;
    fs << "cellSize" << cb.cellSize;
    fs << "resolution" << cb.resolution;
    fs << "}";
    return fs;
}
