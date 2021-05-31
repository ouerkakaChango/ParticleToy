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

	int w, h;
	arr<TraceRay> rays;
};