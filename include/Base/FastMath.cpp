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

P::P(double n) : x(n),y(n),z(n)
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

P P::operator-() const
{
	return P(0, 0, 0) - *this;
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

P P::operator/(const P& p) const
{
	return P(x / p.x, y / p.y, z / p.z);
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

void P::operator*=(const P& p)
{
	x *= p.x;
	y *= p.y;
	z *= p.z;
}

void P::operator/=(double s)
{
	x /= s;
	y /= s;
	z /= s;
}

double P::len() const
{
	return dis(P(0, 0, 0), *this);
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
P operator-(double s, const P& p)
{
	return P(s) - p;
}

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

P safeNorm(const P& p)
{
	double length = len(p);
	if (abs(length) >= 0.00001)
	{
		return p / len(p);
	}
	else
	{
		return P(1, 0, 0);
	}
}

P reflect(const P& i, const P& n)
{
	//https://www.cnblogs.com/graphics/archive/2013/02/21/2920627.html
	return i - 2 * dot(i, n)*n;
}

double dis(const P& p1, const P& p2)
{
	return sqrt(dis2(p1, p2));
}

double dis2(const P& p1, const P& p2)
{
	P d = p2 - p1;
	return dot(d, d);
}

P saturate(const P& p)
{
	return P(
		clamp(p.x, 0, 1),
		clamp(p.y, 0, 1),
		clamp(p.z, 0, 1));
}

P abs(const P& p)
{
	return P(abs(p.x), abs(p.y), abs(p.z));
}

P max(const P& p, double n)
{
	return P(
		p.x > n ? p.x : n,
		p.y > n ? p.y : n,
		p.z > n ? p.z : n
	);
}

P max(double n, const P& p)
{
	return max(p, n);
}

P min(const P& p, double n)
{
	return P(
		p.x < n ? p.x : n,
		p.y < n ? p.y : n,
		p.z < n ? p.z : n
	);
}

P min(double n, const P& p)
{
	return min(p, n);
}

P pow(const P& p1, const P& p2)
{
	return P(pow(p1.x, p2.x), pow(p1.y, p2.y), pow(p1.z, p2.z));
}

P randP()
{
	P re;
	std::default_random_engine random(time(NULL));
	std::uniform_real_distribution<double> dis2(0, std::nextafter(1, DBL_MAX));
	re.x = dis2(random);
	re.y = dis2(random);
	re.z = dis2(random);
	return re;
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

double QuadraFiliter(double x1, double x2, double xmin, double xmax)
{
	bool b1 = x1 >= xmin && x1 <= xmax;
	bool b2 = x2 >= xmin && x2 <= xmax;
	if (b1&&b2)
	{
		if (equal(x1, x2))
		{
			return x1;
		}
		else
		{
			return x1 < x2 ? x1 : x2;
		}
	}
	if (b1)
	{
		return x1;
	}
	else if (b2)
	{
		return x2;
	}
	else
	{
		abort();
	}
	return 0.0;
}

double clamp(double x, double low, double high)
{
	if (high < low)
	{
		abort();
	}
	x = x < low ? low : x;
	x = x > high ? high : x;
	return x;
}

double max(double a, double b)
{
	return a > b ? a : b;
}

double min(double a, double b)
{
	return a < b ? a : b;
}

double sign(double n)
{
	return n < 0 ? -1.0 : 1.0;
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

double equal(P a, P b, double tolerance)
{
	return equal(a.x, b.x, tolerance) &&
		equal(a.y, b.y, tolerance) &&
		equal(a.z, b.z, tolerance);
}

bool zero(double n)
{
	return equal(n, 0.0);
}

bool zero(const P2& p)
{
	return equal(p.x, 0) && equal(p.y, 0);
}

bool zero(const P& p)
{
	return equal(p.x, 0) && equal(p.y, 0) && equal(p.z, 0);
}

double side(double hypotenuse, double side1)
{
	return hypotenuse * hypotenuse - side1 * side1;
}

bool BisecitonSolve(std::function<double(double)> func, double tmin, double tmax, double& t, double tolerance)
{
	bool b1 = func(tmin) > 0;
	bool b2 = func(tmax) > 0;
	if (b1)
	{
		if (b2)
		{
			return false;
		}
		double tem = tmin;
		tmin = tmax;
		tmax = tem;
	}
	else if (!b2)
	{
		return false;
	}
	double tnew = (tmin + tmax) / 2;
	double re = func(tnew);
	if (abs(re) < tolerance)
	{
		t = tnew;
		return true;
	}
	//==0的情况应该已经包含在abs(re)<tolerance里了
	if (func(tnew) > 0)
	{
		BisecitonSolve(func,tmin,tnew,t,tolerance);
	}
	else
	{
		BisecitonSolve(func, tnew, tmax, t, tolerance);
	}
	return true;
}
//### Global Utility