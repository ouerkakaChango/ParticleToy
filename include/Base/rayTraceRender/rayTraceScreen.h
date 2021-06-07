#pragma once
#include "Object.h"

#include "TraceRay.h"

class rayTraceWorld;
class rayTraceScreen : public Object
{
public:
	rayTraceScreen(int w_, int h_);
	void InitRays(rayTraceMode traceMode, rayTraceBounceMode bounceMode, rayTraceMaterialMode matMode);
	void InitBuffers();
	void Trace(rayTraceWorld* world);
	//void GatherInfo(rayTraceWorld* world, const TraceInfo& info, int i, int j);
	void FinalGather();

	void Translate(P offset);

	int w, h;
	arr2<TraceRay> rays;
	arr2<P> debugFrameBuffer;
	arr2<P> normalBuffer;
	arr2<P> colorBuffer;
	arr2<P> posBuffer;

	arr<TraceRay> optRays;

	P pos;
};