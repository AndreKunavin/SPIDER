#pragma once
#include "geom.h"
#include "menu.h"
#include "obstacle.h"
#include "defines.h"
#include <SFML/Graphics.hpp>
using namespace std; 
using sf::Mouse;
using sf::Vertex;
using sf::Vector2f;
using sf::ConvexShape;
using sf::Vertex;
using sf::Keyboard;


bool Button::inside(Point p)
{
	return in_interval(leftup.x, rightdown.x, p.x) &&
		in_interval(rightdown.y, leftup.y, p.y);
	
}

Button::Button(const Point a, const Point b, int (*_handler)(void* params))
{
	handler = _handler;
	rightdown = b;
	leftup = a;
	pict.setPosition((Vector2f)leftup);
}

int Button::doing(void* param)
{
	return handler(param);
}

void Menu::show(RenderWindow& window, Point& whereSpider)
{
	is_menu = 1;
	forBut go;
	go.is_go = 0;  
	go.is_draw = 0;
	back.setPosition(Vector2f(0, 0));
	while (is_menu)
	{
		go.position = whereSpider;//получить go.position из того, где паучок находится
		if (Keyboard::isKeyPressed(Keyboard::Escape))
			is_menu = 0;
		if (Mouse::isButtonPressed(Mouse::Left))
		{
			for (int i = 0; i < (int)buttons.size(); i++)
			{
				if (buttons[i].inside(Point(Mouse::getPosition(window))))
					buttons[i].doing(&go);
			}
		}
	}
}

void Menu::Draw(RenderWindow& window)
{
	for (int i = 0; i < (int)buttons.size(); i++)
	{
		window.draw(buttons[i].pict);
	}
}

bool Roller::inside(Point p)
{
	return p.x > leftup.x&& p.x < rightdown.x &&
		p.y < leftup.y && p.y > rightdown.y;
}

void Roller::setPosition(RenderWindow& window)
{
	left = Point(Vector2f(Mouse::getPosition(window)));
	right = left + Point(len, 0);
}

float Roller::Doing(RenderWindow& window, vector<Obstacle> obs)
{
	setPosition(window);
	float percent;
	while (1)
	{
		if (Keyboard::isKeyPressed(Keyboard::Space))
			return percent;
		while (Mouse::isButtonPressed(Mouse::Left) && inside(Point(Mouse::getPosition(window))))
		{
			if(Mouse::getPosition(window).x < right.x && Mouse::getPosition(window).x > left.x)
				rolic.x = Mouse::getPosition(window).x;
			rolic.y = left.y;
			//как закрасить препятствие?
			percent = float((rolic.x - left.x) / len);
			for (int i = 0; i < (int)obs.size(); i++)
				obs[i].c = sf::Color((int)(255 * percent), (int)(255 * (1 - percent)), 0);
		}
	}
}

void Roller::draw(RenderWindow& window)
{
	board.setPosition(Vector2f(left));
	but.setPosition(Vector2f(leftup));
	window.draw(board);
	window.draw(but);
}

//struct Data
//{
//	RenderWindow& wind;
//	int arr[5];
//};
//int func_do(void* params)
//{
//	Data& data = (Data&)params;	
//	data.wind.draw();
//}