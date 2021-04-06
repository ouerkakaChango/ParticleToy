#pragma once
#include "FastMath.h"

class EffectSpace;
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
	str rule;

	//physic
	double damp = 1.0;
	EffectSpace* effectSpace = nullptr;
};

class Tri
{
public:
	void FromGrid(double len, str filter, bool isUpTri);
	void Transform(P offset);
	P p1, p2, p3;
};