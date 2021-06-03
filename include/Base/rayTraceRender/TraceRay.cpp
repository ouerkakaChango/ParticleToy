#include "TraceRay.h"

#include "rayTraceWorld.h"

TraceRay::TraceRay()
{

}

TraceRay::TraceRay(P a, P b)
{
	line.Set(a, b);
	o = a;
	dir = line.dir();
}

TraceInfo TraceRay::Trace(rayTraceWorld* world)
{
	TraceInfo re;
	if (mode == TraceMode_SphereTracing)
	{
		return SphereTracing(world);
	}
	return re;
}

P TraceRay::Ray(double len)
{
	o = o + dir * len;
	return o;
}

TraceInfo TraceRay::SphereTracing(rayTraceWorld* world)
{
	TraceInfo info = world->SDF(Ray(startLen));
	info.dir = dir;
	while (info.dis > traceThre)
	{
		info = world->SDF(Ray(info.dis));
		if (info.dis >= world->maxSDF)
		{
			return info;
		}
	}
	info.bHit = true;
	info.dir = dir;
	info.hitPos = o;
	info.hitN = info.obj->shape->SDFNormal(o);
	//???
	if (info.obj->name == "Sphere1")
	{
		int aa = 1;
	}
	auto bounceInfo = world->CalculateMaterial(*this, info);
	info.color = bounceInfo.color;
	//???
	if (world->nowBounce == 2)
	{
		int aa = 1;
	}
	if (world->nowBounce == 1)
	{
		color = info.color;
	}
	world->BlendColor(*this,info);
	bStopTrace = bounceInfo.bStopRay;
	//???
	info.debugColor = P(1, 0, 0);
	if (bStopTrace && world->nowBounce == 1)
	{
		info.debugColor = P(0, 0, 1);
	}
	return info;
}

void TraceRay::Bounce(rayTraceBounceMode bounceMode, const TraceInfo& info)
{
	dir = reflect(dir, info.hitN);
}