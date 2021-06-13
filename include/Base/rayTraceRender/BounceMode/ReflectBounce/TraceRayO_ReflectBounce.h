#pragma once
#include "../../TraceRay.h"

class TraceRayO_ReflectBounce : public TraceRayO
{
public:
	TraceRayO_ReflectBounce(TraceRay* y_);
	~TraceRayO_ReflectBounce();
	void FinalUnhitGather() override;
	void FinalHitGather() override;
	void PrepareMaterialExtra(Material& mat) override;
	void InitMaterialPolicy(rayTraceMaterialMode matMode) override;

	arr<double> reflectKs;
	arr<ShadeTask> shadeTasks;
	arr<TraceInfo> traceInfos;

protected:
	void ReflectBounceGather();
};