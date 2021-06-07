#pragma once
#include "TraceRayI_SDFSphere.h"

class TraceRayI_SDFSphereMonteCarlo : public TraceRayI_SDFSphere
{
public:
	TraceRayI_SDFSphereMonteCarlo(TraceRay* y_);

	virtual void Trace(rayTraceWorld* world) override;
	void CreateSubRays(rayTraceWorld* world, const TraceInfo& traceInfo);

	arr<TraceRay> subRays;
	int spp = 128;	//sample per pixel
};