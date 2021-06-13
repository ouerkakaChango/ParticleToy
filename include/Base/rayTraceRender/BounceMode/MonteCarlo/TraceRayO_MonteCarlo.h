#pragma once
#include "../../TraceRay.h"

class TraceRayO_MonteCarlo : public TraceRayO
{
public:
	TraceRayO_MonteCarlo(TraceRay* y_);
	void InitMaterialPolicy(rayTraceMaterialMode matMode) override;
	void FinalHitGather() override;
	ShadeTask shadeTask;
};