#pragma once
#include "../../TraceRay.h"

class TraceRayO_CheapBounce : public TraceRayO
{
public:
	TraceRayO_CheapBounce(TraceRay* y_);

	void FinalUnhitGather() override;
	void PrepareMaterialExtra(Material& mat) override;
	void InitMaterialPolicy(rayTraceMaterialMode matMode) override;

	double lastReflectness = 1.0;
};