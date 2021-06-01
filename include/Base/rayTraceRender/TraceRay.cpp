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
	if (mode == TraceMode_SphereTracing)
	{
		return SphereTracing(world);
	}
}

P TraceRay::Ray(double len)
{
	o = o + dir * len;
	return o;
}

TraceInfo TraceRay::SphereTracing(rayTraceWorld* world)
{
	TraceInfo re;
	double dis = world->SDF(Ray(startLen));
	while (dis > traceThre)
	{
		dis = world->SDF(Ray(dis));
		if (dis >= world->maxSDF)
		{
			return re;
		}
	}
	re.bHit = true;
	re.debugColor = P(1, 0, 0);
	return re;
}