#pragma once
#include "geom.h"

using sf::RenderWindow;
class Obstacle
{
public:
	Polygon border;//многоугольник
	Polygon sfBorder;//массив для рисования (конец не удален)
	sf::Color c = sf::Color::Green;

	bool takeBorders(RenderWindow& window);//заполняет оба массива

	void DrawHull(RenderWindow& window);
	void DrawPolygone(RenderWindow& window);
	void coloringObstacle(RenderWindow& window);//закрашивает препятствие
};

