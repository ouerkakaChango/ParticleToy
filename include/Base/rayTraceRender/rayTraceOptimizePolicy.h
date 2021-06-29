#pragma once
#include "Tools/Timer.h"
#include "Optimize/RTTraceRequest.h"

class rayTraceScreen;
class rayTraceWorld;

class rayTraceOptimizePolicyBase
{
public:
	
};

class rayTraceOptimizePolicy_PerTask : public rayTraceOptimizePolicyBase
{
public:
	void InitTaskRays(rayTraceWorld* world, rayTraceScreen* screen, int loopInx);
	void Trace(rayTraceWorld* world, rayTraceScreen* screen, int loopInx);
	void FinalGather(rayTraceScreen* screen, int loopInx);
	void Clear(rayTraceScreen* screen);

	unsigned int rayPerTask = 0;
	Timer timer;
};

class rayTraceOptimizePolicy_NumbaCUDA : public rayTraceOptimizePolicyBase
{
public:
	void InitRequest(rayTraceWorld* world, rayTraceScreen* screen);
	void Trace(rayTraceWorld* world, rayTraceScreen* screen);

	RTTraceRequest traceReq;
	Timer timer;
};