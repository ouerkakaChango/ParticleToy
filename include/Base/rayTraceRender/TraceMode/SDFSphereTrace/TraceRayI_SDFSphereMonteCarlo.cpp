#include "TraceRayI_SDFSphereMonteCarlo.h"

#include"../../rayTraceWorld.h"

//### TraceRayI_SDFSphereMonteCarlo
int TraceRayI_SDFSphereMonteCarlo::spp = 128;
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

void TraceRayI_SDFSphereMonteCarlo::CreateSubRays(rayTraceWorld* world, const TraceInfo& traceInfo)
{
	//1.创建spp条子Ray,子Ray的i是TraceRayI_SDFSphere，子Ray的o是TraceRayO_ReflectBounce
	//2.子Ray的出射方向是半球上的均匀分布，决定方向的方式参考：https://blog.csdn.net/weixin_44176696/article/details/113418991
	//也就是normalize(randomVec3() + n);
	//子Ray的bounce次数到达

	P a = traceInfo.hitPos;
	subRays.resize(spp);
	for (int i = 0; i < spp; i++)
	{
		P b = a + safeNorm(diskRandP() + traceInfo.hitN);
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
