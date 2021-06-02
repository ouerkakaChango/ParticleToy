#pragma once
#include "Spaces/Spaces.h"

#include "Object.h"
#include "rayTraceScreen.h"
#include "Render/Lights.h"

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
	TraceInfo SDF(P pos);
	P CalculateMaterial(const TraceInfo& info);

	arr<Object*> objs;
	arr<rayTraceScreen*> screens;
	arr<Light*> lights;
	double maxSDF = 1000.0;
};

class rayTraceWorldR : public R
{
	THISR(rayTraceWorld)
	void PutShape(Shape* shape, const str& name="");
	void PutScreen(rayTraceScreen* screen);
	void PutLight(DirectionalLight* light);
	void Evolve();
	void SaveScreenBufferFrame(rayTraceScreen* screen, const str& bufferName, const str& pngPath);
};