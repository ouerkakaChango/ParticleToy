#pragma once 
#include "FastGeo.h"

class Object;
class rayTraceWorld;

enum TraceMode
{
	TraceMode_SphereTracing,
};

class TraceInfo
{
public:
	bool bHit = false;
	P debugColor;
};

class TraceRay
{
public:
	TraceRay();
	TraceRay(P a, P b);
	TraceInfo Trace(rayTraceWorld* world);
	TraceInfo SphereTracing(rayTraceWorld* world);
	P Ray(double len);

	Line line;
	TraceMode mode = TraceMode_SphereTracing;
	double startLen = 5.0;
	double traceThre = 0.01;
	P o,dir;
};