#include "TraceRay.h"

#include "rayTraceWorld.h"

TraceRay::TraceRay(P a, P b)
{
	line = Line(a, b);
}

void TraceRay::Trace(rayTraceWorld* world)
{
	if (mode == TraceMode_SphereTracing)
	{
		SphereTracing(world);
	}
}

void TraceRay::SphereTracing(rayTraceWorld* world)
{

}