#pragma once

#include "headers/obstacle.h"
#include "headers/geom.h"
#include <SFML/Graphics.hpp>

#define STEP 10.0
#define STEP2 1.0

using namespace std;
using sf::Mouse;
using sf::Vertex;
using sf::Vector2f;
using sf::ConvexShape;
using sf::Vertex;


bool Obstacle::takeBorders(RenderWindow&  window)
{
	cout << "Start of building" << endl;
	border.clear();
	Point prev;
	Point prev2;
	while (Mouse::isButtonPressed(Mouse::Left))
	{
		window.display();
		if (border.size() == 0)
		{
			border.push_back(Mouse::getPosition(window));
			sfBorder.push_back(Mouse::getPosition(window));
			prev = border.back();
			prev2 = border.back();
			//cout << border.front()<<endl;
		}
		//cout << "0\n";
		else if (len((Point(Mouse::getPosition(window))) - prev) - STEP > EPS)
		{
			border.push_back(Mouse::getPosition(window));
			//cout << border.back() << "    " << border.size() << endl;
			for (int i = 0; i < ((int)border.size()) - 3; i++)//проверяем, есть ли у кривой самопересечение
			{
				//cout << "dfgsdfghgfdfghgfrhj\n";
				Segment s1 = Segment(border.back(), prev);
				Segment s2 = Segment(border[i], border[i + 1]);//?
				Point res;
				if ((res = get_intersection(s1, s2)) != nunPoint)//если есть пересечение, удалить лишний конец, перестать считывать точки
				{
					for(int k = 0; k < (int)sfBorder.size(); k++)
						if (len(sfBorder[k] - border[i]) < 0.15)
						{
							sfBorder.erase(sfBorder.begin(), sfBorder.begin() + k);
							k = sfBorder.size();
						}
					border.erase(border.begin(), border.begin() + i + 1);//?
					border.back() = res;
					return 1;
				}
			}
			prev = border.back();
		}
		else if (len((Point(Mouse::getPosition(window))) - prev2) - STEP2 > EPS)
		{
			sfBorder.push_back(Mouse::getPosition(window));
			//cout << sfBorder.getVertexCount() << endl;
			DrawHull(window);
			prev2 = Vector2f(Mouse::getPosition(window));
		}
	}
	//если полигон 0, то была нарисована хрень, удаляем
	border.erase(border.begin(), border.end());
	sfBorder.erase(sfBorder.begin(), sfBorder.end());
	return 0;
}

void Obstacle::coloringObstacle(RenderWindow& window)
{
	ConvexShape figur;
	figur.setPointCount((int)sfBorder.size());
	for (int i = 0; i < (int)sfBorder.size(); i++)
	{
		figur.setPoint(i, Vector2f(sfBorder[i]));
		//cout << sfBorder[i].position << endl;
	}
	figur.setFillColor(c);
	window.draw(figur);
}

void Obstacle::DrawHull(RenderWindow& window)
{
	int num = (int)sfBorder.size();
	for (int i = 0; i < num - 1; i++)
	{
		Vertex line[2] =
		{
			Vertex(Vector2f(sfBorder[i])),
			Vertex(Vector2f(sfBorder[i + 1]))
		};
		window.draw(line, 2, sf::Lines);
	}
}

void Obstacle::DrawPolygone(RenderWindow& window)
{
	for (int i = 0; i < ((int)border.size()); i++)
	{
		Vertex line[] =
		{
			Vertex(static_cast<Vector2f>(border[i % border.size()])),
			Vertex(static_cast<Vector2f>(border[(i + 1) % border.size()]))
		};
		window.draw(line, 2, sf::Lines);
	}
}

/*while (window.isOpen())
	{
		sf::Event event;
		window.clear(sf::Color::Blue);
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}
		if (Mouse::isButtonPressed(Mouse::Left))
			takeBorders();	
		DrawPolygone(border);
		DrawHull(sfBorder);
		coloringObstacle();
		window.display();
	}*/