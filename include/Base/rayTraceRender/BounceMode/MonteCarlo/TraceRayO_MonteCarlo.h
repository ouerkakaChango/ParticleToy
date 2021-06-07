#pragma once
#include "../../TraceRay.h"

class TraceRayO_MonteCarlo : public TraceRayO
{
public:
	TraceRayO_MonteCarlo(TraceRay* y_);
	void InitMaterialPolicy(rayTraceMaterialMode matMode) override;
	void CalculateMaterial(rayTraceWorld* world, TraceInfo& info) override;
	//void PrepareMaterialExtra(Material& mat) override;
	void FinalHitGather() override;
	ShadeTask shadeTask;
};