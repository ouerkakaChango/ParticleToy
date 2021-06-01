#pragma once 
#include "FastGeo.h"

class Object;
class rayTraceWorld;
enum TraceMode
{
	TraceMode_SphereTracing,
};
class TraceRay
{
public:
	TraceRay(P a, P b);
	void Trace(rayTraceWorld* world);
	void SphereTracing(rayTraceWorld* world);

	Line line;
	TraceMode mode;
};