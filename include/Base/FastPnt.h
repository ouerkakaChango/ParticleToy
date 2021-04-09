#pragma once
#include "FastMath.h"
#include "EffectSpace.h"

class Pnt
{
public:
	Pnt();
	Pnt(P pos_);
	Pnt(P pos_, str rule_);
	str InfoString(int precision = 3);
	str InfoString(str filter, int precision = 3);
	//### physic
	bool IsBreakPoint() const;
	void SetBreakPoint(const Pnt& pnt, double dtr_);
	Pnt GetBreakPnt() const;
	Pnt GetVirtualOldPnt(double dt) const;
	//### physic

	P pos;
	P v;
	P a;
	str rule;

	//physic
	double damp = 1.0;
	EffectSpace effectSpace;
	Pnt* breakPnt=nullptr;
	bool isBreakPnt = false;
	double dtr = -1.0;
};