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
	while (info.dis > traceThre)
	{
		info = world->SDF(Ray(info.dis));
		if (info.dis >= world->maxSDF)
		{
			return info;
		}
	}
	info.bHit = true;
	info.hitPos = o;
	info.debugColor = P(1, 0, 0);
	info.color = world->CalculateMaterial(info);
	return info;
}