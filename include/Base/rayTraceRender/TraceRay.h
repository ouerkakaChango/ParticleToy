#pragma once 
#include "Taiji.h"

#include "rayTraceUtility.h"
#include "FastGeo.h"
#include "Render/Material.h"
#include "Render/ShadeTask.h"

class Object;
class rayTraceWorld;

class TraceRay
{
	THISY(TraceRay)
	TraceRay(P a, P b);
	void Clear(bool keepColor=false);
	void SetMode(rayTraceMode traceMode, rayTraceBounceMode bounceMode);
	void Trace(rayTraceWorld* world);
	void ShadeAfterHit(rayTraceWorld* world, TraceInfo info);
	P Ray(double len);
	void Bounce(const TraceInfo& info);

	static double startLen;
	static double traceThre;
	P ori,dir;
	P color;
	bool bStopTrace = false;
};

class TraceRayI : public ClassI
{
public:
	TraceRayI(TraceRay* y_);
	virtual ~TraceRayI();
	virtual void Trace(rayTraceWorld* world) = 0;
	virtual void ShadeAfterHit(rayTraceWorld* world, TraceInfo info) = 0;

	TraceRay* y = nullptr;
};

class TraceRayO : public ClassO
{
public:
	TraceRayO(TraceRay* y_);
	virtual ~TraceRayO();
	virtual void InitMaterialPolicy(rayTraceMaterialMode matMode);
	virtual void PrepareMaterialExtra(Material& mat);
	virtual void FinalUnhitGather();
	virtual void FinalHitGather();

	class rayTraceMaterialPolicyBase* matPolicy=nullptr;
	TraceRay* y = nullptr;
};

class rayTraceMaterialPolicyBase
{
public:
	virtual ~rayTraceMaterialPolicyBase() {};

	virtual void BlendColor(rayTraceWorld* world, TraceRay& ray, const TraceInfo& info) = 0;
};

template<class BounceClass, class MaterialClass>
class rayTraceMaterialPolicy : public rayTraceMaterialPolicyBase
{
public:

	void BlendColor(rayTraceWorld* world, TraceRay& ray, const TraceInfo& info) override
	{

	}
};