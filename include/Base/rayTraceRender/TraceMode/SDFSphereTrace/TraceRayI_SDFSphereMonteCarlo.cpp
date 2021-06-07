#include "TraceRayI_SDFSphereMonteCarlo.h"

#include"../../rayTraceWorld.h"

//### TraceRayI_SDFSphereMonteCarlo
TraceRayI_SDFSphereMonteCarlo::TraceRayI_SDFSphereMonteCarlo(TraceRay* y_)
	:TraceRayI_SDFSphere(y_)
{

}

void TraceRayI_SDFSphereMonteCarlo::CreateSubRays(const TraceInfo& traceInfo)
{
	//1.创建spp条子Ray,子Ray的i是TraceRayI_SDFSphere，子Ray的o是TraceRayO_ReflectBounce
	//2.子Ray的出射方向是半球上的均匀分布，决定方向的方式参考：https://blog.csdn.net/weixin_44176696/article/details/113418991
	//也就是normalize(randomVec3() + n);
	//子Ray的bounce次数到达

	P a = traceInfo.hitPos;
	for (int i = 0; i < spp; i++)
	{
		P b = a + norm(randP() + traceInfo.hitN);
		subRays += TraceRay(a, b);
	}
}

void TraceRayI_SDFSphereMonteCarlo::Trace(rayTraceWorld* world)
{
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

		CreateSubRays(info);
	}
	else
	{
		//子Ray的Trace(world)
		//如果nowBounce == bounceNum，收集所有子Ray.color，计算MonteCarlo积分
		//由于决定子Ray的方向是在半球上平均分布的，所以MonteCarlo积分也就是ave(subRay.color*2pi)
		//结果就是间接光照，然后加上直接光照就行了
		if (world->nowBounce == world->bounceNum)
		{
			o->FinalHitGather();
		}
		int aa = 1;
	}
}
//### TraceRayI_SDFSphereMonteCarlo
