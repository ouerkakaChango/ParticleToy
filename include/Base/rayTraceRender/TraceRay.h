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
	void Clear();
	void SetMode(rayTraceMode traceMode, rayTraceBounceMode bounceMode);
	void Trace(rayTraceWorld* world);
	P Ray(double len);
	void Bounce(const TraceInfo& info);

	Line line;
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
	virtual void Trace(rayTraceWorld* world) = 0;

	class TraceRayO* o = nullptr;
	TraceRay* y = nullptr;
};

class TraceRayO : public ClassO
{
public:
	TraceRayO(TraceRay* y_);
	virtual void InitMaterialPolicy(rayTraceMaterialMode matMode);
	virtual void PrepareMaterialExtra(Material& mat);
	virtual void FinalUnhitGather();
	virtual void FinalHitGather();
	//??? 没啥用，之后去掉，都放在matPolicy->BlendColor
	virtual void CalculateMaterial(rayTraceWorld* world, TraceInfo& info);

	class rayTraceMaterialPolicyBase* matPolicy=nullptr;
	TraceRay* y = nullptr;
};

class rayTraceMaterialPolicyBase
{
public:
	virtual void UpdateRayAfterCalculate(TraceRay& ray, const Material& mat) = 0;
	virtual void BlendColor(rayTraceWorld* world, TraceRay& ray, const TraceInfo& info) = 0;
};

template<class BounceClass, class MaterialClass>
class rayTraceMaterialPolicy : public rayTraceMaterialPolicyBase
{
public:
	void UpdateRayAfterCalculate(TraceRay& ray, const Material& mat) override
	{		

	}
	void BlendColor(rayTraceWorld* world, TraceRay& ray, const TraceInfo& info) override
	{

	}
};