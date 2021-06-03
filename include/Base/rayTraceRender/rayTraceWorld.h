#pragma once
#include "Spaces/Spaces.h"

#include "Object.h"
#include "rayTraceScreen.h"
#include "Render/Lights.h"
#include "rayTraceMaterialExtra.h"

class TraceTime : public Ying
{
	THISY(TraceTime)

	int bounceNum = 0;
	int nowBounce = 0;
};

class rayTraceWorld : public TraceTime, public Space3D
{
	THISY(rayTraceWorld)
	void SetTraceSettings(int bounceNum_, rayTraceBounceMode bounceMode_);
	void Evolve();
	TraceInfo SDF(P pos);
	void CalculateMaterial(TraceRay& ray, TraceInfo& info);
	void BlendColor(TraceRay& ray, const TraceInfo& info);

	arr<Object*> objs;
	arr<rayTraceScreen*> screens;
	arr<Light*> lights;
	double maxSDF = 1000.0;
	rayTraceBounceMode bounceMode = rayTraceBounceMode_cheap;

protected:
	void PolicyPrepareMaterialForTrace(Material& mat);
	void PolicyUpdateRayAfterCalculate(TraceRay& ray, const Material& mat);
	void PolicyBlendColor(TraceRay& ray, const Material& mat, const TraceInfo& info);
};

class rayTraceWorldR : public R
{
	THISR(rayTraceWorld)
	Object* PutShape(Shape* shape, const str& name="");
	void PutScreen(rayTraceScreen* screen);
	void PutLight(DirectionalLight* light);
	void Evolve();
	void SaveScreenBufferFrame(rayTraceScreen* screen, const str& bufferName, const str& pngPath);
};