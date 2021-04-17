#pragma once
#include "FastMath.h"
#include "EffectSpace.h"

class Pnt
{
public:
	Pnt();
	Pnt(P pos_);
	Pnt(P pos_, str name_, str rule_);
	void SetSphereOuter(double r);
	str InfoString(int precision = 3);
	str InfoString(str filter, int precision = 3);
	//### physic
	inline void SetMass(double mass_) { mass = mass_; }
	inline void SetVel(const P& v_) { v = v_; }
	bool IsBreakPoint() const;
	void SetBreakPoint(const Pnt& pnt, double dtr_);
	Pnt GetBreakPnt() const;
	Pnt GetVirtualOldPnt(double dt) const;
	//### physic

	P pos;
	str name, rule;
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
	//### physic
};