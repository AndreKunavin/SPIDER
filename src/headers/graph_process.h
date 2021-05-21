
#pragma once
#include "geom.h"
#include <tuple>

using std::vector;
struct Cell
{
	Point center;
	Polygon convex;
};

struct GEdge
{
	Point p = nunPoint;
	int uID = -1;
	std::pair<int, int> cellsID = { -1, -1 };
};


class Graph
{
public:
	vector<Point> coords;
	vector<vector<GEdge>> g;

	void build(const vector<Cell>&, double minDist);
	void add_start_end(Point S, Point E);
	vector<Point> build_way();
	vector<GEdge>& operator[](int index)
	{
		return g[index];
	}
private:
	vector<Cell> mCells;
	bool startEndAdded = false;
	vector<int> modifiedVs;   //Vertexes, those have edges added as edges to End Point
};
 