#pragma once 
#include "Taiji.h"

#include "FastGeo.h"
#include "Render/Material.h"

class Object;
class rayTraceWorld;

enum rayTraceMode
{
	rayTraceMode_SDFSphere,
};

enum rayTraceBounceMode
{
	rayTraceBounceMode_cheap,
};

enum rayTraceMaterialMode
{
	rayTraceMaterialMode_BlinnPhong,
	rayTraceMaterialMode_PBR,
};

class TraceInfo
{
public:
	bool bHit = false;
	P dir;
	P hitPos;
	P hitN;
	double dis=0.0;
	Object* obj = nullptr;

	P color;
};


class TraceRay
{
	THISY(TraceRay)
	TraceRay(P a, P b);
	void SetMode(rayTraceMode traceMode, rayTraceBounceMode bounceMode);
	//TraceRay(P a, P b, rayTraceMode traceMode, rayTraceBounceMode bounceMode);
	TraceInfo Trace(rayTraceWorld* world);
	//TraceInfo SphereTracing(rayTraceWorld* world);
	P Ray(double len);
	void Bounce(const TraceInfo& info);

	Line line;
	double startLen = 0.1;
	double traceThre = 0.01;
	P ori,dir;
	P color,debugColor;
	bool bStopTrace = false;
};

class TraceRayI : public ClassI
{
public:
	virtual TraceInfo Trace(rayTraceWorld* world) = 0;
	virtual void CalculateMaterial(const arr<LightInfo>& lightsInfo, TraceInfo& info);
	class TraceRayO* o = nullptr;
	TraceRay* y = nullptr;
};

class TraceRayO : public ClassO
{
public:
	virtual void InitMaterialPolicy(rayTraceMaterialMode matMode);
	virtual void PrepareMaterialExtra(Material& mat);
	virtual void FinalUnhitGather();

	class rayTraceMaterialPolicyBase* matPolicy=nullptr;
};

class TraceRayI_SDFSphere : public TraceRayI
{
public:
	TraceRayI_SDFSphere(TraceRay* y_);
	virtual TraceInfo Trace(rayTraceWorld* world);
};

class TraceRayO_CheapBounce : public TraceRayO
{
public:
	TraceRayO_CheapBounce(TraceRay* y_);
	void FinalUnhitGather() override;
	void PrepareMaterialExtra(Material& mat) override;
	void InitMaterialPolicy(rayTraceMaterialMode matMode) override;

	TraceRay* y = nullptr;
	double lastReflectness = 1.0;
};

class rayTraceMaterialPolicyBase
{
public:
	virtual void UpdateRayAfterCalculate(TraceRay& ray, const Material& mat) = 0;
	virtual void BlendColor(TraceRay& ray, const TraceInfo& info) = 0;
};

template<class BounceClass, class MaterialClass>
class rayTraceMaterialPolicy : public rayTraceMaterialPolicyBase
{
public:
	void UpdateRayAfterCalculate(TraceRay& ray, const Material& mat) override
	{		

	}
	void BlendColor(TraceRay& ray, const TraceInfo& info) override
	{

	}
};