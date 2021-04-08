#include "FastMath.h"

//### P2
P2::P2(int x_, int y_) :x(x_), y(y_)
{

}
//### P2

//### P
P::P()
{

}

P::P(double x_, double y_, double z_) :x(x_), y(y_), z(z_)
{

}

P::P(int x_, int y_, int z_) :
	x(static_cast<double>(x_)),
	y(static_cast<double>(y_)),
	z(static_cast<double>(z_))
{
}

P::P(P2 p2, str filter)
{
	if (filter == "xz")
	{
		x = p2.x;
		z = p2.y;
	}
}

P P::operator+(const P& p) const
{
	return P(x + p.x, y + p.y, z + p.z);
}

P P::operator-(const P& p) const
{
	return P(x - p.x, y - p.y, z - p.z);
}

P P::operator*(double s) const
{
	return P(x*s, y*s, z*s);
}

P P::operator*(P p) const
{
	return P(x*p.x, y*p.y, z*p.z);
}

P P::operator/(double s) const
{
	return P(x/s, y/s, z/s);
}

void P::operator+=(const P& p)
{
	x += p.x;
	y += p.y;
	z += p.z;
}

void P::operator-=(const P& p)
{
	x -= p.x;
	y -= p.y;
	z -= p.z;
}

//### P

//### Global P
P operator*(double s, const P& p)
{
	return p * s;
}

P cross(const P& p1, const P& p2)
{
	return P(
		p1.y*p2.z - p2.y*p1.z,
		p1.z*p2.x - p2.z*p1.x,
		p1.x*p2.y - p2.x*p1.y
	);
}

double dot(const P& p1, const P& p2)
{
	return p1.x*p2.x + p1.y*p2.y + p1.z*p2.z;
}

double len(const P& p)
{
	return sqrt(p.x*p.x + p.y*p.y + p.z*p.z);
}

P norm(const P& p)
{
	double length = len(p);
	if (abs(length)>=0.00001)
	{
		return p / len(p);
	}
	else
	{
		abort();
		return P(0, 0, 0);
	}
}
//### Global P