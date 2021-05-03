#pragma once
#include "FastMath.h"

class IntersectInfo;

class Shape
{
public:
	Shape();
	virtual IntersectInfo Collide(const Shape& other);
	virtual bool IsPointInside(P pos) const;
	virtual bool InsideOf(const Shape& other) const;
	virtual str TxtHeadString();

	static int uid_count;
	int uid = 0;
};

class IntersectInfo
{
public:
	operator bool() const { return result; }

	bool result = false;
	double d=-1;
	double hitR = -1;
	double r1 = 0.0, r2 = 0.0;
	P hitP;
};

class Line : public Shape
{
public:
	Line();
	Line(P a_, P b_);
	void Set(P a_, P b_);
	void UpdateB(P newb);
	P dir() const;

	friend double len(const Line& l);
	P a, b;
};
double len(const Line& l);

class Sphere :public Shape
{
public:
	Sphere(double r_);
	Sphere(P center_, double r_);
	IntersectInfo Collide(const Shape& other) override;
	bool IsPointInside(P pos) const override;
	bool InsideOf(const Shape& other) const override;
	str TxtHeadString() override;

	double r;
	P center;
};
bool SphereCollide(const Sphere& s1, const Sphere& s2);

class Cylinder : public Line
{
public:
	Cylinder(P a_, P b_, double r_);
	bool IsPointInside(P pos) const override;
	double r;
};

class Capsule : public Line
{
public:
	double r;
	void Set(P a_, P b_, double r_);
	Sphere GetS1() const;
	Sphere GetS2() const;
	Cylinder GetCylinder() const;
};

class Plane : public Shape
{
public:
	Plane();
	Plane(P n_,P p_);
	Plane(const Plane& plane);
	void Define(P n_, P p_);
	void Transform(P offset);
	double sdf(P pos) const;
	IntersectInfo Intersect(const Line& l) const;
	bool IsPointUnder(P pos,const Shape* outer) const;
	P GetFixedPos(P pos, const Shape* outer) const;
	bool IsPointInside(P pos) const override;
	bool IsPointFixed(P pos, const Shape* outer) const;
	P p, n;
	bool isDefined = false;
};
P project(P pos, const Plane& plane);

class Tri:public Plane
{
public:
	Tri();
	Tri(const Tri& tri);
	Tri(const P& p1_, const P& p2_, const P& p3_);
	void FromGrid(double len, str filter, bool isUpTri);
	void Scale(double s);
	void Transform(P offset);
	void CalculateNormal();
	bool IsPointInside(P pos) const override;
	IntersectInfo Collide(const Line& l) const;
	IntersectInfo Collide(const Cylinder& cylinder) const;
	IntersectInfo Collide(const Capsule& cap) const;
	IntersectInfo Collide(const Sphere& s) const;
	P p1, p2, p3;
};

class ShapeHull
{
public:
	bool IsPointInside(P pos);
	arr<Shape*> hulls;
};
ShapeHull M_Add(const Tri& tri, const Sphere& s);