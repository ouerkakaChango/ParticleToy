#pragma once
#include "FastMath.h"

class EffectSpace;
class IntersectInfo;

class Pnt
{
public:
	Pnt();
	Pnt(P pos_);
	Pnt(P pos_, str rule_);
	str InfoString(int precision = 3);
	//### physic
	void EffectUpdate(const Pnt& prev);
	//### physic

	P pos;
	P v;
	P a;
	str rule;

	//physic
	double damp = 1.0;
	EffectSpace* effectSpace = nullptr;
};

class Line
{
public:
	P a, b;
	void Set(P a_, P b_);
	void Update(P newb);
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