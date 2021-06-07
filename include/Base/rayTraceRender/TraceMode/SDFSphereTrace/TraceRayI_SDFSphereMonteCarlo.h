#pragma once
#include "TraceRayI_SDFSphere.h"

class TraceRayI_SDFSphereMonteCarlo : public TraceRayI_SDFSphere
{
public:
	TraceRayI_SDFSphereMonteCarlo(TraceRay* y_);
	~TraceRayI_SDFSphereMonteCarlo();

	virtual void Trace(rayTraceWorld* world) override;
	void CreateSubRays(rayTraceWorld* world, const TraceInfo& traceInfo);

	arr<TraceRay> subRays;
	static int spp;	//sample per pixel
};