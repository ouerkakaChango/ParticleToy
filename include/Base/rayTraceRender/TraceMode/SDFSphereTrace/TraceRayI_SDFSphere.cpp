#include "TraceRayI_SDFSphere.h"

#include "../../rayTraceWorld.h"

//### TraceRayI_SDFSphere
TraceRayI_SDFSphere::TraceRayI_SDFSphere(TraceRay* y_):
	TraceRayI(y_)
{

}

TraceRayI_SDFSphere::~TraceRayI_SDFSphere()
{

}

void TraceRayI_SDFSphere::Trace(rayTraceWorld* world)
{
	auto o = Cast<TraceRayO*>(y->o[0]);
	TraceInfo info = world->SDF(y->Ray(y->startLen));
	info.dir = y->dir;
	while (info.dis > y->traceThre)
	{
		info = world->SDF(y->Ray(info.dis));
		if (info.dis >= world->maxSDF)
		{
			//没有这个>1的判断也可以，不过省的(0,0,0)和(0,0,0)lerp了。
			if (world->nowBounce > 1)
			{
				o->FinalUnhitGather();
			}
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

	if (world->nowBounce == world->bounceNum)
	{
		y->bStopTrace = true;
		o->FinalHitGather();
	}
	if (world->nowBounce < world->bounceNum)
	{
		y->Bounce(info);
	}
	return;
}

void TraceRayI_SDFSphere::ShadeAfterHit(rayTraceWorld* world, arr<TraceInfo>& infos)
{
	auto& info = infos[0];
	if (!info.bHit)
	{
		y->bStopTrace = true;
		return;
	}
	info.hitN = info.obj->shape->SDFNormal(y->ori);

	int count = 1;
	while (zero(info.hitN))
	{
		//有可能发生，获取SDF Normal失败
		//那就获取上一个位置的SDF Normal
		info.hitN = info.obj->shape->SDFNormal(info.hitPos - info.dir*0.00001*count);
		count++;
	}

	arr<LightInfo> lightsInfo = world->GetLightsInfo(info.hitPos);
	auto o = Cast<TraceRayO*>(y->o[0]);
	o->matPolicy->BlendColor(world, *y, info);

	if (world->nowBounce == world->bounceNum)
	{
		y->bStopTrace = true;
		o->FinalHitGather();
	}
	if (world->nowBounce < world->bounceNum)
	{
		y->Bounce(info);
	}
	return;
}
//### TraceRayI_SDFSphere