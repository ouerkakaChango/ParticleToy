#pragma once 
#include "FastGeo.h"

class Object;
class rayTraceWorld;

enum TraceMode
{
	TraceMode_SphereTracing,
};

enum rayTraceBounceMode
{
	rayTraceBounceMode_cheap,
};

class TraceInfo
{
public:
	bool bHit = false;
	P dir;
	P hitPos;
	P hitN;
	double dis=0.0;
	Object* obj = nullptr;

	P debugColor;
	P color;
};

class TraceRay
{
public:
	TraceRay();
	TraceRay(P a, P b);
	TraceInfo Trace(rayTraceWorld* world);
	TraceInfo SphereTracing(rayTraceWorld* world);
	P Ray(double len);
	void Bounce(rayTraceBounceMode bounceMode, const TraceInfo& info);

	Line line;
	TraceMode mode = TraceMode_SphereTracing;
	double startLen = 5.0;
	double traceThre = 0.01;
	P o,dir;
	P color;
};