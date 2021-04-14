#include "FastMath.h"

#include <random>
//### P2
P2::P2(int x_, int y_) :x(x_), y(y_)
{

}

P2::P2(double x_, double y_) : x(x_), y(y_)
{

}

P2 P2::operator+(const P2& p) const
{
	return P2(x + p.x, y + p.y);
}

P2 P2::operator-(const P2& p) const
{
	return P2(x - p.x, y - p.y);
}

P2 P2::operator*(double s) const
{
	return P2(x*s, y*s);
}

P2 P2::operator/(double s) const
{
	return P2(x/s, y/s);
}

void P2::operator+=(const P2& p)
{
	x += p.x;
	y += p.y;
}

bool P2::operator>=(const P2& p) const
{
	return x >= p.x && y >= p.y;
}

bool P2::operator<(const P2& p) const
{
	return x < p.x && y < p.y;
}

bool P2::operator==(const P2& p) const
{
	return x == p.x && y == p.y;
}

str P2::ToStr()
{
	str re;
	re += x;
	re += " ";
	re += y;
	return re;
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
	else if (filter == "zx")
	{
		z = p2.x;
		x = p2.y;
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

void P::operator*=(double s)
{
	x *= s;
	y *= s;
	z *= s;
}

str P::ToStr()
{
	str re;
	re += x;
	re += " ";
	re += y;
	re += " ";
	re += z;
	return re;
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

P reflect(const P& i, const P& n)
{
	//https://www.cnblogs.com/graphics/archive/2013/02/21/2920627.html
	return i - 2 * dot(i, n)*n;
}
//### Global P

//### Global Utility
bool SolveQuadra(double a, double b, double c, double& x1, double& x2)
{
	if (abs(a) < 0.00001)
	{
		return false;
	}
	else 
	{
		double delta = b * b - 4 * a*c;
		if (delta < 0)
		{
			return false;
		}
		else
		{
			double invDeno = 0.5 / a;
			delta = sqrt(delta);
			x1 = invDeno * (-b + delta);
			x2 = invDeno * (-b - delta);
		}
	}
	return true;
}

bool SolveQuadra(P a, P b, P c, double& x1, double& x2)
{
	bool success = false;
	if (!SolveQuadra(a.x, b.x, c.x, x1, x2))
	{
		if (!SolveQuadra(a.y, b.y, c.y, x1, x2))
		{
			if (!SolveQuadra(a.z, b.z, c.z, x1, x2))
			{
				return false;
			}
		}
	}
	return true;
}

double rand01()
{
	static std::default_random_engine e;        // 生成无符号随机整数
	// 0 到 1 （包含）的均匀分布
	static std::uniform_real_distribution<double>u(0, 1);
	return u(e);
}

double equal(double a, double b, double tolerance)
{
	return std::abs(a - b) < tolerance;
}

bool zero(double n)
{
	return equal(n, 0.0);
}

bool zero(const P2& p)
{
	return equal(p.x, 0) && equal(p.y, 0);
}
//### Global Utility