#pragma once
#include "FastMath.h"

class IntersectInfo;

class Line
{
public:
	P a, b;
	void Set(P a_, P b_);
	void UpdateB(P newb);
	P dir() const;

	friend double len(const Line& l);
};
double len(const Line& l);

class Plane
{
public:
	P p, n;
	void Define(P n_, P p_);
	void Transform(P offset);
	IntersectInfo Intersect(const Line& l) const;
};

class Tri:public Plane
{
public:
	void FromGrid(double len, str filter, bool isUpTri);
	void Scale(double s);
	void Transform(P offset);
	void CalculateNormal();
	bool IsPointInside(P p) const;
	IntersectInfo Intersect(const Line& l) const;
	P p1, p2, p3;
};

class IntersectInfo
{
public:
	bool hit = false;
	double d;
	P hitP;
};