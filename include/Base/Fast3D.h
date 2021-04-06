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
	double x=0.0, y=0.0, z=0.0;

	P operator*(double s) const;
	P operator*(P p) const;
	//P operator*(Filter p) const;
	P operator+(const P& p);
	P operator-(const P& p);
	void operator+=(const P& p);
	friend P operator*(double s, const P& p);
};

P operator*(double s, const P& p);

class Tri
{
public:
	void FromGrid(double len, str filter, bool isUpTri);

	P p1, p2, p3;
};

class Pnt
{
public:
	Pnt();
	Pnt(P pos_);
	Pnt(P pos_, str rule_);
	P pos;
	str InfoString(int precision=3);
	str rule;
	double damp = 1.0;
};

//??? 一个完整高效的三维空间解决方案，看齐VDB,PCL那种
class Fast3D
{
public:
	void AddPnt(str name, P pos, str rule);
	str InfoString(int precision=3);
	arr<Pnt> pnts;
};