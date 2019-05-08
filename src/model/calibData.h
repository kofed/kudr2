#include "opencv2/opencv.hpp"
#include <utility>

using namespace cv;
using namespace std;

class Surface{
private:
    //расстояние между плоскостями введенное оператором
    const int h;
    //координаты центра. Введены оператором
    const Point2f centerL, centerR;

    //chess board with disparity
    ChessBoard leftPx;
    ChessBoard rightPx;
    ChessBoard leftSm;
    ChessBoard rightSm;

    //угол на левой/правой камере
    vector<Corner> corners;
public:
Surface::Surface(const int & _h, const Point2f & _centerL, const Point2f & _centerR, const vector<pair<Point2f, Point2f>> & _corners)
    :centerL(_centerL), centerR(_centerR), corners(_corners), h(_h){

}
    void toYml(FileStorage & yml) const {
    	yml << "h" << h;
    	yml << "corners" << "[";

    	for(auto c : corners){
                yml << "{:" << "pointL" << c.first << "pointR" << c.second;
    	}

		yml << "]";
    }

    static Surface fromYml(FileNodeIterator & fn){
        float _h;
        (*fn)["h"] >> _h;
        Point2f _centerL, _centerR;
        (*fn)["centerL"] >> _centerL;
        (*fn)["centerR"] >> _centerR;

        vector<pair<Point2f, Point2f>> corners;
        FileNode fnCorners = (*fn)["corners"];
    	for(FileNodeIterator it = fnCorners.begin() ; it != fnCorners.end(); ++it){
            Point2f left, right;
            (*it)["pointL"] >> left;
            (*it)["pointR"] >> right;
            corners.push_back(make_pair(left, right));
    	}
        return Surface(_h, _centerL, _centerR, corners);
    }
};

//модель для сохранения в yml
class CalibData{
public:

    //размер клетки шахматной доски мм
    int cellSize;
    //высота между плоскостями к углам
    vector<Surface> surfaces;

    CalibData(){

    }

    CalibData(const int _cellSize):
                cellSize(_cellSize){

    }

    void toYml(FileStorage & yml) const{
        yml << "cellSize" << cellSize;
        yml << "surfaces" << "[";
        for(auto s : surfaces){
            s.toYml(yml);
        }
        yml << "]";
    }

    static CalibData fromYml(const string & name){
        FileStorage yml(name, FileStorage::WRITE);
    	if(!yml.isOpened()){
    		throw runtime_error("не могу загрузить калибровочные данные. Файл не открыт");
    	}

        float h;
        int cellSize;
        Point2i center;
    	yml["cellSize"] >> cellSize;
    	yml["center"] >> center;

        CalibData cs(cellSize);

    	FileNode fnCorners = yml["corners"];
    	for(FileNodeIterator it = fnCorners.begin() ; it != fnCorners.end(); ++it){
                cs.surfaces.push_back(Surface::fromYml(it));
    	}
    }
};

