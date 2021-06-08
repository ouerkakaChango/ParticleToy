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
			//û�����>1���ж�Ҳ���ԣ�����ʡ��(0,0,0)��(0,0,0)lerp�ˡ�
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
	o->CalculateMaterial(world, info);
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