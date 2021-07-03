#pragma once
#include "../../TraceRay.h"

class TraceRayI_SDFSphere : public TraceRayI
{
public:
	TraceRayI_SDFSphere(TraceRay* y_);
	virtual ~TraceRayI_SDFSphere();
	virtual void Trace(rayTraceWorld* world) override;
	virtual void ShadeAfterHit(rayTraceWorld* world, arr<TraceInfo>& infos) override;
};