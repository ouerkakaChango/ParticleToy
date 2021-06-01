#include "TraceRay.h"

#include "rayTraceWorld.h"

TraceRay::TraceRay(P a, P b)
{
	line.Set(a, b);
	o = a;
	dir = line.dir();
}

void TraceRay::Trace(rayTraceWorld* world)
{
	if (mode == TraceMode_SphereTracing)
	{
		SphereTracing(world);
	}
}

P TraceRay::Ray(double len)
{
	o = o + dir * len;
	return o;
}

void TraceRay::SphereTracing(rayTraceWorld* world)
{
	double dis = world->SDF(Ray(startLen));
	while (dis > traceThre)
	{
		dis = world->SDF(Ray(dis));
		if (dis >= world->maxSDF)
		{
			return;
		}
	}
	int aa = 0;
}