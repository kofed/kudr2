class ChessBoard{

private:
	//sorted corners
	vector<vector<Point2f>> corners;

	

public:
	ChessBoard(const vector<vector<Point2f>> & corners);

	Point2f get(Point2i index);
	
	const Size size;

	toYml();

	static ChessBoard fromYml();
};
