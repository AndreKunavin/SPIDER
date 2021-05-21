#include "headers/geom.h"
#include <map>
#include "headers/defines.h"
#include "headers/obstacle.h"
#include "headers/menu.h"

using namespace sf;
using namespace std;
//кнопки: 
//запустить паучка
//остановить паучка
//пустить паучка заново
//ролик с опасностью
//разрешить рисование/закончить рисование
//очистить карту
//отменить последнее препятствие

vector <Obstacle> obstacles;

void Create_buts(vector<Button>& buts);

int Go(void* param)
{
	cout << "go!";
	forBut& fb = (forBut&)param;
	fb.is_go = 1;
	return 1;
}

int Stop(void* param)
{
	cout << "stop";
	forBut& fb = (forBut&)param;
	fb.is_go = 0;
	return 0;
}

int Restart(void* param)
{
	forBut& fb = (forBut&)param;
	fb.is_go = 0;
	fb.position = (Point)START;
	return 0;
}

int Create_obstacle(void* param)
{
	forBut& fb = (forBut&)param;
	fb.is_go = !(fb.is_go);
	return bool(fb.is_go);
}

int NewField(void* param)
{
	obstacles.clear();
	return 0;
}

int RemoveTheLast(void* param)
{
	if((int)obstacles.size())
		obstacles.pop_back();
	return 1;
}

int main()
{
	Menu menu;
	Point a;
	Create_buts(menu.buttons);
	RenderWindow window(sf::VideoMode(WEIDHT, HIGHT), "my_window");
	while (window.isOpen())
	{
		sf::Event event;
		window.clear(sf::Color::Blue);
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}
		menu.Draw(window);
		if(Mouse::isButtonPressed(Mouse::Left))
			menu.show(window, a);
		window.display();
	}
	return 0;
}


void Create_buts(vector<Button>& buts)
{
	
	double y1 = 20;
	double y2 = 40;
	double x = 0, dx = 20, butLen = 20;
	buts.push_back(Button(Point(WEIDHT / 17, y1), Point(WEIDHT * 3 / 34, y2), Go));
	buts.push_back(Button(Point(WEIDHT * 2 / 17, y1), Point(WEIDHT * 5 / 34, y2), Stop));
	buts.push_back(Button(Point(WEIDHT * 3 / 17, y1), Point(WEIDHT * 7 / 34, y2), Restart));
	buts.push_back(Button(Point(WEIDHT * 4 / 17, y1), Point(WEIDHT * 9 / 34, y2), Create_obstacle));
	buts.push_back(Button(Point(WEIDHT * 5 / 17, y1), Point(WEIDHT * 11 / 34, y2), NewField));
	buts.push_back(Button(Point(WEIDHT * 6 / 17, y1), Point(WEIDHT * 13 / 34, y2), RemoveTheLast));
	//buts[1] = Button(Point(WEIDHT * 7 / 17, y1), Point(WEIDHT * 15 / 34, y2), Stop);
	
	for (int i = 0; i < buts.size(); i++)
	{
		buts[i].leftup = Point(dx * (2 * i + 1), y1);
		buts[i].rightdown = Point(dx * (2 * i + 2), y2);
	}
	vector<string> strs(buts.size(), "../res/");
	for (int i = 0; i < strs.size(); i++)
	{
		strs[i] = strs[i] + (char)('0' + 1 + i) + ".jpg";
		buts[i].texture.loadFromFile(strs[i]);
		buts[i].pict.setTexture(buts[i].texture);
		buts[i].pict.setPosition(Vector2f(buts[i].leftup));
	}

	return;
}






