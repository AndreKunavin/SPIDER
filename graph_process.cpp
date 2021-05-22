#include "headers/graph_process.h"
#include <set>

struct GVertex
{
	GVertex() {}
	Point p = nunPoint;
	int vID = -1;
	vector<GEdge> edges;
	int cellID = -1;
};

void Graph::build(const vector<Cell>& cells, double minDist)
{
	vector<GVertex> pts;
	int curCellID = -1;
	int curVertexID = 0;
	for (const auto& cell : cells)
	{
		curCellID++;
		int sz = cell.convex.size();
		for (int i = 0; i < sz; i++)
		{
			GVertex tmpV;
			tmpV.p = cell.convex[i];
			tmpV.cellID = curCellID;
			tmpV.vID = curVertexID;
			
			GEdge ePrev, eNext;
			if (i == sz - 1)
			{
				eNext.p = cell.convex.front();
				eNext.uID = curVertexID - sz + 1;
				ePrev.p = cell.convex[i - 1];
				ePrev.uID = curVertexID - 1;
			}
			else if (i == 0)
			{
				eNext.p = cell.convex[i + 1];
				eNext.uID = curVertexID + 1;
				ePrev.p = cell.convex.back();
				ePrev.uID = curVertexID + sz - 1;
			}
			else
			{
				eNext.p = cell.convex[i + 1];
				eNext.uID = curVertexID + 1;
				ePrev.p = cell.convex[i - 1];
				ePrev.uID = curVertexID - 1;
			}
			//std::cout << "Vertex " << curVertexID << " " << eNext.uID << std::endl;
			tmpV.edges.push_back(ePrev);
			tmpV.edges.push_back(eNext);
			pts.push_back(tmpV);
			curVertexID++;
		}
	}

	vector<GVertex> pts0 = pts;
	sort(pts.begin(), pts.end(), [](const GVertex& a, const GVertex& b)
		{
			return abs(a.p.x - b.p.x) > EPS ? (a.p.x < b.p.x) : (a.p.y < b.p.y);
		});

	int i = 0;
	int cnt = 0;
	while (i < pts.size())
	{
		g.push_back(vector<GEdge>());

		coords.push_back(pts[i].p);
		pts0[pts[i].vID].vID = cnt;
		vector<GEdge> curEdges;
		curEdges.push_back(pts[i].edges.back());
		curEdges.push_back(pts[i].edges.front());
		
		int j = i + 1;
		while ((j < pts.size()) && (len(pts[i].p - pts[j].p) < EPS))
		{
			curEdges.push_back(pts[j].edges.back());
			curEdges.push_back(pts[j].edges.front());
			pts0[pts[j].vID].vID = cnt;
			j++;
		}

		sort(curEdges.begin(), curEdges.end(), [&](const GEdge& a, const GEdge& b)
			{
				double ang1 = angle({ 1, 0 }, a.p - pts[i].p);
				if (ang1 < 0)
					ang1 = 2 * M_PI - ang1;
				double ang2 = angle({ 1, 0 }, b.p - pts[i].p);
				if (ang2 < 0)
					ang2 = 2 * M_PI - ang2;

				return ang1 < ang2;
			});


		for (int k = 0; k < curEdges.size(); k++)
		{
			int next = (k + 1) % (int)curEdges.size();
			int prev = (k == 0) ? (int)curEdges.size() - 1 : k - 1;
			std::pair<GEdge, GEdge> eqEdges;

			double mlen = abs(len(curEdges[k].p - curEdges[next].p));
			if (abs(len(curEdges[k].p - curEdges[next].p)) < EPS)
				eqEdges = { curEdges[k], curEdges[next] };
			else if (abs(len(curEdges[k].p - curEdges[prev].p)) > EPS)
			{
				GEdge nunEdge;
				nunEdge.uID = -1;
				eqEdges = { curEdges[k], nunEdge };
			}
			else continue;
		
			GEdge newEdge;
			newEdge.p = curEdges[k].p;
			newEdge.uID = curEdges[k].uID;

			int cell1ID = pts0[eqEdges.first.uID].cellID;
			int cell2ID = eqEdges.second.uID == -1 ? -1 : pts0[eqEdges.second.uID].cellID;
			Vector v = newEdge.p - pts[i].p;
			Point start = pts[i].p;
			

			newEdge.cellsID = { cell1ID, cell2ID };
			if (vector_work(v, cells[cell1ID].center - start) < 0)
				std::swap(newEdge.cellsID.first, newEdge.cellsID.second);


			Segment seg(newEdge.p, pts[i].p);
			int existCellId = newEdge.cellsID.first == -1 ? newEdge.cellsID.second : newEdge.cellsID.first;
			if (dist(seg, cells[existCellId].center) * 2 > minDist)
				g[cnt].push_back(newEdge);
		
		}

		cnt++;
		i = j;
	}

	for (auto& arr : g)
		for (GEdge& edge : arr)
		{
			edge.uID = pts0[edge.uID].vID;
		}

	mCells = cells;
}



void Graph::add_start_end(Point S, Point E)
{
	//cleaning of previous graph modifying
	if (startEndAdded)
	{
		for (int id : modifiedVs)
			g[id].pop_back();
		g.pop_back();
		g.pop_back();
		coords.pop_back();
		coords.pop_back();
		modifiedVs.clear();
	}

	//Find cell of start Point and of End Point
	double minValS = INF, minValE = INF;
	int startCellID, endCellID;
	for (int i = 0; i < mCells.size(); i++)
	{
		double curLen = 0;
		if ((curLen = len(S - mCells[i].center)) < minValS)
		{
			minValS = curLen;
			startCellID = i;
		}
		if ((curLen = len(E - mCells[i].center)) < minValE)
		{
			minValE = curLen;
			endCellID = i;
		}
	}

	vector<GEdge> vS;
	for (int i = 0; i < coords.size(); i++)
	{
		for (Point p : mCells[startCellID].convex)
			if (len(p - coords[i]) < EPS)
			{
				GEdge newEdge;
				newEdge.cellsID = { startCellID, startCellID };
				newEdge.p = p;
				newEdge.uID = i;
				vS.push_back(newEdge);
			}
		for (Point p : mCells[endCellID].convex)
			if (len(p - coords[i]) < EPS)
			{
				GEdge newEdge;
				newEdge.cellsID = { endCellID, endCellID };
				newEdge.p = E;
				newEdge.uID = coords.size() + 1;
				g[i].push_back(newEdge);

				modifiedVs.push_back(i);
			}
	}

	coords.push_back(S);
	coords.push_back(E);
	g.push_back(vS);
	g.push_back(vector<GEdge>());  //symbolic end point

	startEndAdded = true;
}


vector<Point> Graph::build_way()
{
	vector<Point> answ;
	if (!startEndAdded)
		return answ;

	//Deijstra
	//from point coords.size - 2 to point coords.size - 1  way is to be found

	int sz = coords.size();
	auto cmp = [](std::pair<double, int> a, std::pair<double, int>  b) 
	{
		return a.first - b.first < -EPS;
	};
	std::set<std::pair<double, int>, decltype(cmp)> vSet(cmp); //dist for point : id of 
	vSet.insert({ 0, sz - 2 });

	vector<bool> used(sz, false); // if vertex is used already
	used[sz - 2] = true;
	vector<double> dists(sz, INF); //distances for vertexes
	dists[sz - 2] = 0;
	vector<int> prev(sz);  // ancestor v for this v
	prev[sz - 2] = sz - 2;

	while(!vSet.empty())
	{
		std::pair<double, int> curV = *vSet.begin();
		vSet.erase(vSet.begin());
		used[curV.second] = true;

		for (GEdge edge : g[curV.second])
		{
			if (!used[edge.uID])
			{
				double curDist = dists[curV.second] + len(coords[curV.second] - edge.p);
				if (dists[edge.uID] == INF)
				{
					dists[edge.uID] = curDist;
					vSet.insert({ curDist, edge.uID });
					prev[edge.uID] = curV.second;
				}
				else if ((curDist < dists[edge.uID]))
				{
					vSet.erase({ dists[edge.uID], edge.uID});
					dists[edge.uID] = curDist;
					vSet.insert({curDist, edge.uID});
					prev[edge.uID] = curV.second;
				}
				else continue;
			}
		}
	}

	if (dists[sz - 1] == INF)
		return answ;

	//know obtaining of way
	int vID = sz - 1;
	answ.push_back(coords[vID]);
	while (vID != prev[vID])
	{
		vID = prev[vID];
		answ.push_back(coords[vID]);
	}

	std::reverse(answ.begin(), answ.end());

	return answ;
}
	