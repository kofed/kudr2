class ChessBoard{

private:
	//sorted corners
	vector<vector<Point2f>> corners;

	Size size;

public:
	ChessBoard(const vector<vector<Point2f>> & corners);

	Point2f get(Point2i index);

	toYml();

	static ChessBoard fromYml();
};
