#pragma once
#include "FastMath.h"

class IntersectInfo;

class Shape
{
public:
	virtual bool Collide(const Shape* other);
	virtual str TxtHeadString();

	str type;
};

class IntersectInfo
{
public:
	bool hit = false;
	double d;
	P hitP;
};

class Line : public Shape
{
public:
	P a, b;
	void Set(P a_, P b_);
	void UpdateB(P newb);
	P dir() const;

	friend double len(const Line& l);
};
double len(const Line& l);

class Capsule : public Line
{
public:
	double r;
	void Set(P a_, P b_, double r_);
};

class Plane : public Shape
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
	Tri();
	Tri(const P& p1_, const P& p2_, const P& p3_);
	void FromGrid(double len, str filter, bool isUpTri);
	void Scale(double s);
	void Transform(P offset);
	void CalculateNormal();
	bool IsPointInside(P p) const;
	IntersectInfo Intersect(const Line& l) const;
	P p1, p2, p3;
};

class Sphere:public Shape
{
public:
	Sphere(double r_);
	bool Collide(const Shape* other) override;;
	str TxtHeadString() override;

	double r;
	P center;
};
bool Intersect(const Sphere& s1, const Sphere& s2);