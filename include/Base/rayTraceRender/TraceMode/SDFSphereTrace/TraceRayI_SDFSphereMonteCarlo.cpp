#include "TraceRayI_SDFSphereMonteCarlo.h"

#include"../../rayTraceWorld.h"

//### TraceRayI_SDFSphereMonteCarlo
TraceRayI_SDFSphereMonteCarlo::TraceRayI_SDFSphereMonteCarlo(TraceRay* y_)
	:TraceRayI_SDFSphere(y_)
{

}

void TraceRayI_SDFSphereMonteCarlo::CreateSubRays(const TraceInfo& traceInfo)
{
	//1.����spp����Ray,��Ray��i��TraceRayI_SDFSphere����Ray��o��TraceRayO_ReflectBounce
	//2.��Ray�ĳ��䷽���ǰ����ϵľ��ȷֲ�����������ķ�ʽ�ο���https://blog.csdn.net/weixin_44176696/article/details/113418991
	//Ҳ����normalize(randomVec3() + n);
	//��Ray��bounce��������

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
		//��Ray��Trace(world)
		//���nowBounce == bounceNum���ռ�������Ray.color������MonteCarlo����
		//���ھ�����Ray�ķ������ڰ�����ƽ���ֲ��ģ�����MonteCarlo����Ҳ����ave(subRay.color*2pi)
		//������Ǽ�ӹ��գ�Ȼ�����ֱ�ӹ��վ�����
		if (world->nowBounce == world->bounceNum)
		{
			o->FinalHitGather();
		}
		int aa = 1;
	}
}
//### TraceRayI_SDFSphereMonteCarlo
