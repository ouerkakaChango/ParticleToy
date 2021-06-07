#pragma once 
#include "Taiji.h"

#include "FastGeo.h"
#include "Render/Material.h"
#include "Render/ShadeTask.h"

class Object;
class rayTraceWorld;

enum rayTraceMode
{
	rayTraceMode_SDFSphere,
};

enum rayTraceBounceMode
{
	rayTraceBounceMode_cheap,	//仅使用单方向(trace反射方向)作为IndirectLight，通过材质reflectness混合反射
	rayTraceBounceMode_reflect,	//和cheap几乎一样，但将indirect点作为点光源进入光照模型计算，混合更物理
	rayTraceBounceMode_MonteCarlo, //经典蒙特卡洛路径追踪，也就是只考虑2trace多条采样（比如spp32）,而不是n*n。也算是reflect升级版。
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
	void Trace(rayTraceWorld* world);
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