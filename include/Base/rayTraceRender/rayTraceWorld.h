#pragma once
#include "Spaces/Spaces.h"
#include "Object.h"
#include "rayTraceScreen.h"

class TraceTime : public Ying
{
	THISY(TraceTime)

	int bounceNum=0;
};

class rayTraceWorld : public TraceTime, public Space3D
{
	THISY(rayTraceWorld)
	void SetTraceSettings(int bounceNum_);
	void Evolve();
	double SDF(P pos);

	arr<Object*> objs;
	arr<rayTraceScreen*> screens;
	double maxSDF = 1000.0;
};

class rayTraceWorldR : public R
{
	THISR(rayTraceWorld)
	void PutShape(Shape* shape, const str& name="");
	void PutScreen(rayTraceScreen* screen);
	void Evolve();
};