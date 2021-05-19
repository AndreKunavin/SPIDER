#include "headers\geom.h"


bool in_interval(const double &a, const double &b, const double &x)
{
	return x >= a && x <= b || x >= b && x <= a;
}


	Point::Point(const double& _x, const double& _y)
	{
		x = _x;
		y = _y;
	}

	template <typename T>
	Point::Point(const Vector2<T>& v)
	{
		x = (double)v.x;
		y = (double)v.y;
	}


	double Point::len() const
	{
		return sqrt(x * x + y * y);
	}

	Point Point::normalize() const
	{
		return Point(x / len(), y / len());
	}

	Point Point::to_orthogonal() const
	{
		return Point(-y, x);
	}

	Point Point::move(const Point &v, const double &d) const
	{
		return *this + v.normalize() * d;
	}

	Point Point::operator-(const Point& b) const
	{
		return{ x - b.x, y - b.y };
	}

	Point Point::operator+(const Point& b) const
	{
		return{ x + b.x, y + b.y };
	}

	Point Point::operator*(const double& val) const
	{
		return{ x * val, y * val };
	}

	bool Point::operator==(const Point &b) const
	{
		return abs(x - b.x) <= EPS && abs(y - b.y) <= EPS;
	}

	bool Point::operator!=(const Point &b) const
	{
		return !(*this == b);
	}

	Point Point::operator-() const
	{
		return{ -x, -y };
	}


	double len(Point a)
	{
		return a.len();
	}

double scalar_work(const Point &a, const Point &b)
{
	return a.x * b.x + a.y * b.y;
}

double vector_work(const Point &first, const Point &second)
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

	Line::Line(const Point &A, const Point &B)
	{
		a = B.y - A.y;
		b = A.x - B.x;
		c = B.x * A.y - A.x * B.y;
	}

	Line::Line(const double &_a, const double &_b, const double &_c) : a(_a), b(_b), c(_c) {}

	bool Line::include(const Point &p) const
	{
		return abs(p.x * a + p.y * b + c) < EPS;
	}



double dist(const Line &line, const Point &p)
{
	return abs(line.a * p.x + line.b * p.y + line.c) / hypot(line.a, line.b);
}


	Segment::Segment(const Point &S, const Point &E) : Line(S, E), S(S), E(E) {}

	bool Segment::include(const Point &p) const
	{
		return Line::include(p) && in_interval(S.x, E.x, p.x) && in_interval(S.y, E.y, p.y);
	}


double dist(const Segment &seg, const Point &p)
{
	if (scalar_work(seg.E - seg.S, p - seg.S) >= 0 && scalar_work(seg.S - seg.E, p - seg.E) >= 0)
		return dist(Line(seg.a, seg.b, seg.c), p);
	else
		return std::min(len(p - seg.S), len(p - seg.E));
}


	bool Circle::operator==(const Circle &c) const
	{
		return this->center == c.center && this->r == c.r;
	}

	bool Circle::operator!=(const Circle &c) const
	{
		return !(*this == c);
	}

	int Circle::include(const Point &p) const
	{
		double d = len(p - center);
		if (r != d)
			return d > r ? 1 : -1;
		return 0;
	}

	void Circle::get_tangent_points(const Point &p, Point &p1, Point &p2) const
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

typedef std::vector<Point> Polygon;


std::istream& operator >> (std::istream& in, Point& p)
{
	in >> p.x >> p.y;
	return in;
}

std::ostream& operator << (std::ostream &out, const Point &p)
{
	out << p.x << " " << p.y;
	return out;
}

std::istream& operator >> (std::istream &in, Line &line)
{
	in >> line.a >> line.b >> line.c;
	return in;
}

std::ostream& operator << (std::ostream &out, const Line &line)
{
	out << line.a << " " << line.b << " " << line.c;
	return out;
}

std::istream& operator >> (std::istream &in, Circle &circle)
{
	in >> circle.center >> circle.r;
	return in;
}

std::istream& operator >> (std::istream &in, Segment &seg)
{
	Point a, b;
	in >> a >> b;
	seg = Segment(a, b);
	return in;
}

std::ostream& operator << (std::ostream &out, const std::vector<Point> &p)
{
	for (auto const &cur : p)
		out << cur << std::endl;
	return out;
}


Point get_intersection(const Line &l1, const Line &l2)
{
	if (abs(l1.a * l2.b - l1.b * l2.a) < EPS)
		return nunPoint;

	Point p;
	p.y = (l2.a * l1.c - l1.a * l2.c) / (l1.a * l2.b - l2.a * l1.b);
	p.x = (l1.b * l2.c - l2.b * l1.c) / (l1.a * l2.b - l2.a * l1.b);
	return p;
}

Point get_intersection(const Segment& s1, const Segment& s2)
{
	double val1 = vector_work(s1.E - s1.S, s2.E - s1.S) * vector_work(s1.E - s1.S, s2.S - s1.E);
	double val2 = vector_work(s2.E - s2.S, s1.E - s2.S) * vector_work(s2.E - s2.S, s1.S - s2.E);
	if ((val1 > EPS) || (val2 > EPS))
		return nunPoint;
	if ((val1 < -EPS) && (val2 < -EPS))
		return get_intersection((Line)s1, (Line)s2);
	if (s1.include(s2.S))
		return s2.S;
	if (s1.include(s2.E))
		return s2.E;
	if (s2.include(s1.S))
		return s1.S;
	if (s2.include(s1.E))
		return s1.E;
	return nunPoint;
} //Аня хочет спать

std::vector<Point> get_intersection(const Circle &circle, const Line &line)
{
	std::vector<Point> rez;

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

std::vector<Point> get_intersection(Circle &c1, Circle &c2)
{
	std::vector<Point> rez;
	if (c1.r > c2.r)
		std::swap(c1, c2);

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

std::vector<Point> get_convex_hull(Polygon &v)
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
	for (int i = 1; i < (int)v.size(); i++)
	{
		while (rez.size() > 1 && vector_work(rez[rez.size() - 1] - rez[rez.size() - 2], v[i] - rez[rez.size() - 1]) <= 0)
			rez.pop_back();

		rez.push_back(v[i]);
	}

	return rez;
}
