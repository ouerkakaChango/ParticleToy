#pragma once
#include "Spaces/Spaces.h"
#include "Object.h"

class TraceTime : public Ying
{
	THISY(TraceTime)

	int bounceNum=0;
};

class rayTraceWorld : public TraceTime, public Space3D
{
	THISY(rayTraceWorld)
	void SetTraceSettings(int bounceNum_);
};

class rayTraceWorldR : public R
{
	THISR(rayTraceWorld)
	void PutShape(Shape* shape, const str& name="");

	arr<Object*> objs;
};