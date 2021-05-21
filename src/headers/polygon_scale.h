#pragma once
#include "geom.h"

//whether center in nunPoint, then mass center is used
Polygon scale_polygon(const Polygon& p, double k, Point center = nunPoint)
{
	center = { 0, 0 };
	for (Point v : p)
		center += v;
	center = center / (double)p.size();

	Polygon answ;
	for (Point v : p)
		answ.push_back((v - center) * k + center);
	return answ;
}
