#pragma once
#include "Container.h"

class P2
{
public:
	P2() {}
	P2(int x_, int y_);
	P2(double x_, double y_);
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

bool SolveQuadra(double a, double b, double c, double& x1, double& x2);
bool SolveQuadra(P a, P b, P c, double& x1, double& x2);