#pragma once
#include "Object.h"

#include "TraceRay.h"

class rayTraceWorld;
class rayTraceScreen : public Object
{
public:
	rayTraceScreen(int w_, int h_);
	void InitRays();
	void Trace(rayTraceWorld* world);
	void GatherInfo(const TraceInfo& info, int i, int j);

	int w, h;
	arr2<TraceRay> rays;
	arr2<P> debugFrameBuffer;
	arr2<P> colorBuffer;
};