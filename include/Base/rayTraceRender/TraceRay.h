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
	P Ray(double len);

	Line line;
	TraceMode mode = TraceMode_SphereTracing;
	const double startLen = 5.0;
	const double traceThre = 0.01;
	P o,dir;
};