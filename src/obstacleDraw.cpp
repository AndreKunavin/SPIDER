#include <iostream>
#include <SFML\Graphics.hpp>
#include "headers\geom.h"
#include <conio.h>
#include "graph_process.h"


#define WIDTH 800
#define HEIGHT 600
using namespace sf;
double a = 30,
	   h = a * sqrt(3) / 2,
	   k = 5;

Point center = { 0, a / 2 };
Polygon rect = 
{
	{ -a / 2, 0 },
	{ -a / 2, a },
	{ a / 2, a },
	{ a / 2, 0} 
};

Polygon rect2 =
{
	{ -a / 2 * k, 0 },
	{ -a / 2 * k, a },
	{ a / 2 * k, a },
	{ a / 2 * k, 0 }
};


Cell move_cell(const Cell& cell, Vector v)
{
	Cell answ = cell;
	answ.center = answ.center + v;
	for (Point& cur : answ.convex)
		cur = cur + v;
	return answ;
}

Graph Test1(int cntx, int cnty, double minDist)
{
	vector<Cell> cells;
	Cell baseCell{ center, rect };
	Vector baseV = { WIDTH / 2, HEIGHT / 2 };
	for (int i = 0; i < cntx; i++)
		for (int j = 0; j < cnty; j++)
			cells.push_back(move_cell(baseCell, baseV + Vector(a, 0) * i + Vector(0, a) * j));

	Graph answerG;
	answerG.build(cells, minDist);
	return answerG;
}

Graph Test2(int cntx, int cnty, double minDist)
{
	vector<Cell> cells;
	Cell baseCell{ center, rect2 };
	Vector baseV = { WIDTH / 2, HEIGHT / 2 };
	for (int i = 0; i < cntx; i++)
		for (int j = 0; j < cnty; j++)
			cells.push_back(move_cell(baseCell, baseV + Vector(a * k, 0) * i + Vector(0, a) * j));

	Graph answerG;
	answerG.build(cells, minDist);
	return answerG;
}
/*
int main()
{
	Point p = Point(Mouse::getPosition());
	Graph g = Test2(3, 6, a);
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "My window");
	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}

		window.clear(sf::Color::Red);

		for (int i = 0; i < g.g.size(); i++)
			for (GEdge edge : g.g[i])
			{
				Vertex segm[2];
				segm[0] = ((Vector2f)g.coords[i]);
				segm[1] = ((Vector2f)g.coords[edge.uID]);
				window.draw(segm, 2, sf::Lines);
			}


		window.display();
	}

	_getch();
	return 0;
}
*/