#pragma once
#include "Container.h"

class P2
{
public:
	P2(int x_, int y_);
	int x, y;
};

class P
{
public:
	P();
	P(double x_, double y_, double z_);
	P(int x_, int y_, int z_);
	P(P2 p2, str filter);
	double x = 0.0, y = 0.0, z = 0.0;

	P operator*(double s) const;
	P operator*(P p) const;
	//P operator*(Filter p) const;
	P operator+(const P& p) const;
	P operator-(const P& p) const;
	void operator+=(const P& p);
	void operator-=(const P& p);
	friend P operator*(double s, const P& p);
};

P operator*(double s, const P& p);