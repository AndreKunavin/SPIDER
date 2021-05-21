#pragma once
#include "geom.h"
#include <map>
#include "defines.h"
#include "obstacle.h"
using namespace std;
using sf::RenderWindow;
using sf::Sprite;
using sf::Texture;

class Button
{
	typedef void DoFunc ();
public:
	Point rightdown;
	Point leftup;
	Sprite pict;
	Texture texture;

	Button(const Point a, const Point b, int (*_handler)(void* param));
	bool inside(Point p);

	int doing(void* param);
	
private:
	int (*handler) (void* param);
};

class Menu
{
public:
	vector<Button> buttons;
	bool is_menu;
	Sprite back;
	void Draw(RenderWindow& window);
	void show(RenderWindow& window, Point& whereSpider);
};

class Roller : public Button
{
public:
	Point left, right;
	float len;
	Sprite board;

	Point rightdown;
	Point leftup;
	Point rolic;
	Sprite but;

	void draw(RenderWindow& window);
	void setPosition(RenderWindow& window);
	float Doing(RenderWindow& window, vector<Obstacle> obs);
	bool inside(Point p);
};

struct forBut
{
	bool is_go;
	bool is_draw;
	Point position;
};

//нужны кнопки: 
//запустить паучка
//остановить паучка
//пустить паучка заново
//ролик с опасностью
//разрешить рисование
//сложно...