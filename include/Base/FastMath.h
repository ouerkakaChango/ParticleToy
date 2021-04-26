#pragma once
#include "Container.h"

class P2
{
public:
	P2() {}
	P2(int x_, int y_);
	P2(double x_, double y_);
	P2 operator+(const P2& p) const;
	P2 operator-(const P2& p) const;
	P2 operator*(double s) const;
	P2 operator/(double s) const;
	void operator+=(const P2& p);
	bool operator>=(const P2& p) const;
	bool operator<(const P2& p) const;
	bool operator==(const P2& p) const;
	inline int IntX() const { return static_cast<int>(x); }
	inline int IntY() const { return static_cast<int>(y); }
	inline int RoundX() const { return static_cast<int>(x + 0.5); }
	inline int RoundY() const { return static_cast<int>(y + 0.5); }
	str ToStr();
	double x=0.0, y=0.0;
};

class P
{
public:
	P();
	P(double x_, double y_, double z_);
	P(int x_, int y_, int z_);
	P(P2 p2, str filter);
	double x = 0.0, y = 0.0, z = 0.0;

	P operator+(const P& p) const;
	P operator-(const P& p) const;
	P operator*(double s) const;
	P operator*(P p) const;
	P operator/(double s) const;
	void operator+=(const P& p);
	void operator-=(const P& p);
	void operator*=(double s);
	friend P operator*(double s, const P& p);

	str ToStr();
};

P operator*(double s, const P& p);
P cross(const P& p1, const P& p2);
double dot(const P& p1, const P& p2);
double len(const P& p);
P norm(const P& p);
P reflect(const P& i, const P& n);
double dis(const P& p1, const P& p2);
double dis2(const P& p1, const P& p2);

bool SolveQuadra(double a, double b, double c, double& x1, double& x2);
bool SolveQuadra(P a, P b, P c, double& x1, double& x2);
double rand01();
double equal(double a, double b, double tolerance = 0.00001);
bool zero(double n);
bool zero(const P2& p);
double side(double hypotenuse, double side1);
template <class T,class T2>
T lerp(T a, T b, T2 k)
{
	return a + k * (b - a);
}
template <class T, class T2>
T lerpRate(T a, T b, T2 re)
{
	//a+k*(b-a)=re
	return (re - a) / (b - a);
}