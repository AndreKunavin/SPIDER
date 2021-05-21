#pragma once
#include "geom.h"

using sf::RenderWindow;
class Obstacle
{
public:
	Polygon border;//�������������
	Polygon sfBorder;//������ ��� ��������� (����� �� ������)
	sf::Color c = sf::Color::Green;

	bool takeBorders(RenderWindow& window);//��������� ��� �������

	void DrawHull(RenderWindow& window);
	void DrawPolygone(RenderWindow& window);
	void coloringObstacle(RenderWindow& window);//����������� �����������
};

