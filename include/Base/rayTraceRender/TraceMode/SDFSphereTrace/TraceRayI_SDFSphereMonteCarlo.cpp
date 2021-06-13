#include "TraceRayI_SDFSphereMonteCarlo.h"

#include"../../rayTraceWorld.h"

//### TraceRayI_SDFSphereMonteCarlo
int TraceRayI_SDFSphereMonteCarlo::spp = 128;
rayTraceSampleMode TraceRayI_SDFSphereMonteCarlo::sampleMode = rayTraceSampleMode_UniformSampling;

TraceRayI_SDFSphereMonteCarlo::TraceRayI_SDFSphereMonteCarlo(TraceRay* y_)
	:TraceRayI_SDFSphere(y_)
{

}

TraceRayI_SDFSphereMonteCarlo::~TraceRayI_SDFSphereMonteCarlo()
{
	for (auto& ray : subRays)
	{
		ray.Clear();
	}
}

P TraceRayI_SDFSphereMonteCarlo::RandSampleDir(const TraceInfo& traceInfo)
{
	if (sampleMode == rayTraceSampleMode_UniformSampling)
	{
		//如果是均匀sample,不用考虑材质的影响
		//子Ray的出射方向是半球上的均匀分布，决定方向的方式参考：https://blog.csdn.net/weixin_44176696/article/details/113418991
		return safeNorm(diskRandP() + traceInfo.hitN);
	}
	else if (sampleMode == rayTraceSampleMode_ImportanceSampling)
	{
		auto matParam = Cast<MaterialI*>(traceInfo.obj->material->i[0]);
		return Cast<MaterialO*>(traceInfo.obj->material->o[0])->ImportanceRandSampleDir(matParam, traceInfo.hitN, -traceInfo.dir);
	}
	abort();
	return P(0.0);
}

void TraceRayI_SDFSphereMonteCarlo::CreateSubRays(rayTraceWorld* world, const TraceInfo& traceInfo)
{
	//1.创建spp条子Ray,子Ray的i是TraceRayI_SDFSphere，子Ray的o是TraceRayO_ReflectBounce

	P a = traceInfo.hitPos;
	subRays.resize(spp);
	for (int i = 0; i < spp; i++)
	{
		P b = a + RandSampleDir(traceInfo);
		subRays[i] = TraceRay(a, b);
		subRays[i].SetMode(rayTraceMode_SDFSphere, rayTraceBounceMode_reflect);
		Cast<TraceRayO*>(subRays[i].o[0])->InitMaterialPolicy(world->matMode);
	}
}

void TraceRayI_SDFSphereMonteCarlo::Trace(rayTraceWorld* world)
{
	auto o = Cast<TraceRayO*>(y->o[0]);
	if (world->nowBounce == 1)
	{
		TraceInfo info = world->SDF(y->Ray(y->startLen));
		info.dir = y->dir;
		while (info.dis > y->traceThre)
		{
			info = world->SDF(y->Ray(info.dis));
			if (info.dis >= world->maxSDF)
			{
				y->bStopTrace = true;
				return;
			}
		}
		info.bHit = true;
		info.dir = y->dir;
		info.hitPos = y->ori;
		info.hitN = info.obj->shape->SDFNormal(y->ori);

		arr<LightInfo> lightsInfo = world->GetLightsInfo(info.hitPos);
		o->matPolicy->BlendColor(world, *y, info);

		if (world->bounceNum > 1)
		{
			CreateSubRays(world, info);
		}
	}
	else
	{
		for (auto& subRay : subRays)
		{
			subRay.Trace(world);
		}
	}
	if (world->nowBounce == world->bounceNum)
	{
		o->FinalHitGather();
	}
}
//### TraceRayI_SDFSphereMonteCarlo
