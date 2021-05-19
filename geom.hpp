#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <random>


using namespace std;

const double EPS = 1e-5;
const double INF = 1e10;

bool in_interval(const double &a, const double &b, const double &x)
{
	return x >= a && x <= b || x >= b && x <= a;
}

struct Point
{
	double x, y;

	Point(const double &_x = INF, const double &_y = INF) : x(_x), y(_y) {};

	double len() const
	{
		return sqrt(x * x + y * y);
	}

/////////*****************************//////////
	Point rand(double square) 
	{
        std::random_device rd;
        std::mt19937 gen((rd()));
        std::uniform_real_distribution<> distr(-square, square);
        double x = distr(gen);
        double y = distr(gen);
        return Point(x, y);
    }
////////*******************************/////////

	Point normalize() const
	{
		return Point(x / len(), y / len());
	}

	Point to_orthogonal() const
	{
		return Point(-y, x);
	}

	Point move(const Point &v, const double &d) const
	{
		return *this + v.normalize() * d;
	}

	Point operator-(const Point& b) const
	{
		return {x - b.x, y - b.y};
	}

	Point operator+(const Point& b) const
	{
		return {x + b.x, y + b.y};
	}

	Point operator*(const double& val) const
	{
		return {x * val, y * val};
	}

	bool operator==(const Point &b) const
	{
		return abs(x - b.x) <= EPS && abs(y - b.y) <= EPS;
	}

	bool operator!=(const Point &b) const
	{
		return !(*this == b);
	}

	Point operator-() const
	{
		return {-x, -y};
	}

} const nunPoint(INF, INF);

typedef Point Vector;

double len(Vector a)
{
	return sqrt(a.x * a.x + a.y * a.y);
}

double scalar_work(const Vector &a, const Vector &b)
{
	return a.x * b.x + a.y * b.y;
}

double vector_work(const Vector &first, const Vector &second)
{
	return first.x * second.y - first.y * second.x;
}

double angle(const Vector &v1, const Vector &v2)
{
	return atan2(vector_work(v1, v2), scalar_work(v1, v2));
}

Point move(const Point &p, const Vector &v, double const &len)
{
	return p + v.normalize() * len;
}

struct Line
{
	double a, b, c;

	Line(const Point &A, const Point &B)
	{
		a = B.y - A.y;
		b = A.x - B.x;
		c = B.x * A.y - A.x * B.y;
	}

	Line(const double &_a = 0, const double &_b = 0, const double &_c = 0) : a(_a), b(_b), c(_c) {};

	virtual bool include(const Point &p) const
	{
		return abs(p.x * a + p.y * b + c) < EPS;
	}
};

double dist(const Line &line, const Point &p)
{
	return abs(line.a * p.x + line.b * p.y + line.c) / hypot(line.a, line.b);
}

struct Segment : public Line
{
	Point S, E;

	Segment(const Point &S = 0, const Point &E = 0) : Line(S, E), S(S), E(E) {};
//////***********************************************//////
	Line midpoint_perpendicular() 
	{
		double a, b, c;
		a = 2 * (E.x - S.x);
		b = 2 * (E.y - S.y);
		c = (-1) * ((E.x * E.x - S.x * S.x) + (E.y * E.y - S.y * S.y));
		Line perp(a, b, c);
		return perp;
	}
//////////*******************************************///////
	virtual bool include(Point &p) const
	{
		return Line::include(p) && in_interval(S.x, E.x, p.x) && in_interval(S.y, E.y, p.y);
	}
};

double dist(const Segment &seg, const Point &p)
{
	if (scalar_work(seg.E - seg.S, p - seg.S) >= 0 && scalar_work(seg.S - seg.E, p - seg.E) >= 0)
		return dist(Line(seg.a, seg.b, seg.c), p);
	else
		return min(len(p - seg.S), len(p - seg.E));
}

struct Circle
{
	Point center;
	double r;

	bool operator==(const Circle &c) const
	{
		return this->center == c.center && this->r == c.r;
	}

	bool operator!=(const Circle &c) const
	{
		return !(*this == c);
	}

	int include(const Point &p) const
	{
		double d = len(p - center);
		if (r != d)
			return d > r ? 1 : -1;
		return 0;
	}

	void get_tangent_points(const Point &p, Point &p1, Point &p2) const
	{
		if (include(p) == -1)
		{
			p1 = p2 = nunPoint;
			return;
		}
		if (include(p) == 0)
		{
			p1 = p2 = p;
			return;
		}

		double c, d, h;
		c = len(center - p);
		d = r * r / c;
		h = sqrt(r * r - d * d);

		Point m = center.move(p - center, d);
		Vector n = (center - p).to_orthogonal();
		p1 = m.move(n, h);
		m.move(-n, h);

		return;
	}
};

typedef vector<Point> Polygon;

istream& operator >> (istream& in, Point& p)
{
	in >> p.x >> p.y;
	return in;
}

ostream& operator << (ostream &out, const Point &p)
{
	out << p.x << " " << p.y;
	return out;
}

istream& operator >> (istream &in, Line &line)
{
	in >> line.a >> line.b >> line.c;
	return in;
}

ostream& operator << (ostream &out, const Line &line)
{
	out << line.a << " " << line.b << " " << line.c;
	return out;
}

istream& operator >> (istream &in, Circle &circle)
{
	in >> circle.center >> circle.r;
	return in;
}

istream& operator >> (istream &in, Segment &seg)
{
	Point a, b;
	in >> a >> b;
	seg = Segment(a, b);
	return in;
}

ostream& operator << (ostream &out, const vector<Point> &p)
{
	for (auto const &cur : p)
		out << cur << endl;
	return out;
}

Point get_intersection(const Line &l1, const Line &l2)
{
	if (abs(l1.a * l2.b - l1.b * l2.a) < EPS) 
	{
		return nunPoint;
	}
	Point p;
	p.y = (l2.a * l1.c - l1.a * l2.c) / (l1.a * l2.b - l2.a * l1.b);
	p.x = (l1.b * l2.c - l2.b * l1.c) / (l1.a * l2.b - l2.a * l1.b);
	return p;
}

vector<Point> get_intersection(const Circle &circle, const Line &line)
{
	vector<Point> rez;

	Point M = circle.center;
	double r = circle.r;
	double d = dist(line, M);

	if (d > r)
	{
		return rez;
	}

	Point N = M.move(Vector(line.a, line.b), d);

	if (!line.include(N))
		N = M.move(Vector(-line.a, -line.b), d);

	Vector n = Vector(line.a, line.b).to_orthogonal();
	d = sqrt(r * r - d * d);

	Point answ1, answ2;
	answ1 = N.move(n, d);
	answ2 = N.move(-n, d);

	rez.push_back(answ1);
	if (answ1 != answ2)
		rez.push_back(answ2);

	return rez;
}

vector<Point> get_intersection(Circle &c1, Circle &c2)
{
	vector<Point> rez;
	if (c1.r > c2.r)
		swap(c1, c2);

	double d2, d;
	d = len(c1.center - c2.center);

	if (c1.r + d < c2.r || d > c1.r + c2.r)
		return rez;

	if (d == c1.r + c2.r || d + c1.r == c2.r)
	{
		rez.push_back(c2.center.move(c1.center - c2.center, c2.r));
		return rez;
	}

	d2 = (d * d + c2.r * c2.r - c1.r * c1.r) / (2 * d);
	double h = sqrt(c2.r * c2.r - d2 * d2);

	Point p = c2.center.move(c1.center - c2.center, d2);
	Vector n = (c1.center - c2.center).to_orthogonal();

	rez.push_back(p.move(n, h));
	rez.push_back(p.move(-n, h));

	return rez;
}

/////**************************************////// intersection of half-plane and a vertex ///// Voronoi special //// return value: 1 - yes; -1 - no;
std::size_t intersection(Line border, Point site, Point vertex) {
	if (line_point_att(border, site) == line_point_att(border, vertex)) {
		return 1;
	} else { 
		return -1;
	}
}
double line_point_att(Line l, Point s) {
	return (l.a * s.x + l.b * s.y + l.c >= 0) ? 1 : -1;
}
/////**************************************//////

vector<Point> get_convex_hull(Polygon &v)
{
	Point edgePoint = v[0];
	for (Point &cur : v)
		if ((cur.y < edgePoint.y) || (cur.y == edgePoint.y && cur.x > edgePoint.x))
			edgePoint = cur;
	Vector Ox = { 1, 0 };

	sort(v.begin(), v.end(), [edgePoint, Ox](const Point &a, const Point &b)
	{
		double angl1 = angle(Ox, a - edgePoint),
			angl2 = angle(Ox, b - edgePoint);
		return angl1 != angl2 ? angl1 < angl2 : len(a - edgePoint) < len(b - edgePoint);
	});

	Polygon rez({ v[0] });
	for (int i = 1; i < v.size(); i++)
	{
		while (rez.size() > 1 && vector_work(rez[rez.size() - 1] - rez[rez.size() - 2], v[i] - rez[rez.size() - 1]) <= 0)
			rez.pop_back();

		rez.push_back(v[i]);
	}

	return rez;
}


