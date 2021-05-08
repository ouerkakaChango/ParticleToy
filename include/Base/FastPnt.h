#pragma once
#include "FastMath.h"
#include "EffectSpace.h"
#include "Relation.h"

class Pnt
{
public:
	Pnt();
	Pnt(P pos_);
	Pnt(P pos_, str name_, str rule_);
	void UpdateEffectSpace();
	//### Shape
	void SetSphereOuter(double r);
	void UpdateOuter() const;
	IntersectInfo Collide(const Pnt& pnt);
	//### Shape
	//### info
	str InfoString(int precision = 5);
	str InfoString(str filter, int precision = 5);
	str TxtHeadString();
	//### info
	//### physic
	bool IsBreakPoint() const;
	void SetBreakPoint(const Pnt& pnt, double dtr_);
	Pnt GetBreakPnt() const;
	void AddRestLength(int pntInx,double restLen);
	//### physic

	P pos;
	str name, rule;
	int inxOffset = 0;
	Shape* outer = nullptr;
	//### physic
	P v;
	P a;
	double mass = 1.0;
	double damp = 1.0;
	double dtr = -1.0;
	EffectSpace effectSpace;
	Pnt* breakPnt=nullptr;
	bool isBreakPnt = false;
	bool ignorePrev = false;
	arr<Relation*> relations;
	//int fuckYour18Generation = 1;
	//### physic
};

bool RuleOf(str rule, str prop);