#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <SFML/Graphics.hpp>

const double EPS = 1e-5;
const double INF = 1e10;

using std::istream;
using std::ostream;

bool in_interval(const double &a, const double &b, const double &x);



struct Point : sf::Vector2<double>
{
	Point(const double& _x = INF, const double& _y = INF);

	template <typename T>
	Point(const Vector2<T>& v);
	

	double len() const;

	Point normalize() const;

	Point to_orthogonal() const;

	Point move(const Point &v, const double &d) const;

	Point operator-(const Point& b) const;

	Point operator+(const Point& b) const;

	Point operator*(const double& val) const;

	bool operator==(const Point &b) const;

	bool operator!=(const Point &b) const;

	Point operator-() const;

	friend istream& operator >> (std::istream& in, Point& p);
	friend ostream& operator << (std::ostream &out, const Point &p);

} const nunPoint(INF, INF);
	
typedef Point Vector;

double len(Point a);

double scalar_work(const Point &a, const Point &b);

double vector_work(const Point &first, const Point &second);

double angle(const Vector &v1, const Vector &v2);

Point move(const Point &p, const Vector &v, double const &len);

struct Line
{
	double a, b, c;

	Line(const Point &A, const Point &B);

	Line(const double &_a = 0, const double &_b = 0, const double &_c = 0);

	virtual bool include(const Point &p) const;

	friend istream& operator >> (std::istream &in, Line &line);


	friend ostream& operator << (std::ostream &out, const Line &line);
};

double dist(const Line &line, const Point &p);

struct Segment : public Line
{
	Point S, E;

	Segment(const Point &S = 0, const Point &E = 0);

	virtual bool include(const Point &p) const;

	friend istream& operator >> (std::istream &in, Segment &seg);
};

double dist(const Segment &seg, const Point &p);

struct Circle
{
	Point center;
	double r;

	bool operator==(const Circle &c) const;

	bool operator!=(const Circle &c) const;

	int include(const Point &p) const;

	void get_tangent_points(const Point &p, Point &p1, Point &p2) const;

	friend istream& operator >> (std::istream &in, Circle &circle);
};

typedef std::vector<Point> Polygon;


std::istream& operator >> (std::istream& in, Point& p);

std::ostream& operator << (std::ostream &out, const Point &p);

std::istream& operator >> (std::istream &in, Line &line);

std::ostream& operator << (std::ostream &out, const Line &line);

std::istream& operator >> (std::istream &in, Circle &circle);

std::istream& operator >> (std::istream &in, Segment &seg);

std::ostream& operator << (std::ostream &out, const std::vector<Point> &p);


Point get_intersection(const Line &l1, const Line &l2);

Point get_intersection(const Segment& s1, const Segment& s2);

std::vector<Point> get_intersection(const Circle &circle, const Line &line);

std::vector<Point> get_intersection(Circle &c1, Circle &c2);

std::vector<Point> get_convex_hull(Polygon &v);
