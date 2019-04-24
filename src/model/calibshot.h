class Surface{
private:
    //расстояние между плоскостями введенное оператором
    const int h;

    //угол на левой/правой камере
    map<Point2f, Point2f> corners;

    Surface(){};
public:
    Surface(const Point2f & _pointL, const Point2f & _pointR);

    void toYml(FileStorage & yml) const {
    	yml << "h" << h;
    	yml << "corners" << "[";

    	for(auto c : corners){
    		yml << "{:" << "pointL" << c->first << "pointR" << c->second;
    	}

		yml << "]";
    }

    static Surface fromYml(FileNode & fn){
    	Surface surface;
    	fn["h"] >> h;

    	FileNode fnCorners = yml["corners"];
    	for(FileNodeIterator it = fnCorners.begin() ; it != fnCorners.end(); ++it){
    		surface.corners[*it["pointL"]] = *it["pointR"];
    	}
    	return surface;
    }
};

//модель для сохранения в yml
class CalibData{
private:
	CalibData(){};
public:

    //размер клетки шахматной доски мм
    const int cellSize;
    //координаты центра px
    const Point2i center;

    //высота между плоскостями к углам
    vector<Surface> surfaces;

    Point3f getPointSm(const Point2f lPoint, const float disparity) const{
    	// height(Point2f, disparity)
    	//map[Point3, h]
    	//find closest

    	//calculate Point2f px to Point2f in sm
    	//pointSm(Point px, disparity)
    }

    CalibShot(const int _h, const int _cellSize, const Point2i _center):h(_h),
    		cellSize(_cellSize), center(_center){

    }

    void toYml(FileStorage & yml){
        yml << "h" << h;
        yml << "cellSize" << cellSize;
        yml << "center" << center;
        yml << "corners" << "[";
        for(auto c : corners){
            c.toYml(yml);
        }
        yml << "]";
    }

    static CalibShot fromYml(string & name){
    	CalibShot cs;

    	FileStorage yml(name, FileStorage::WRITE);
    	if(!yml.isOpened()){
    		throw runtime_error("не могу загрузить калибровочные данные. Файл не открыт");
    	}

    	yml["h"] >> h;
    	yml["cellSize"] >> cellSize;
    	yml["center"] >> center;

    	FileNode fnCorners = yml["corners"];
    	for(FileNodeIterator it = fnCorners.begin() ; it != fnCorners.end(); ++it){
    		cs.push_back(CalibCorner::fromYml(*it));
    	}
    }
};

