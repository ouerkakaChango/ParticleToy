#include "TraceRay.h"

#include "rayTraceWorld.h"
#include "TraceMode/SDFSphereTrace/TraceRayI_SDFSphere.h"
#include "TraceMode/SDFSphereTrace/TraceRayI_SDFSphereMonteCarlo.h"
#include "BounceMode/CheapBounce/TraceRayO_CheapBounce.h"
#include "BounceMode/ReflectBounce/TraceRayO_ReflectBounce.h"
#include "BounceMode/MonteCarlo/TraceRayO_MonteCarlo.h"

//### TraceRay
double TraceRay::startLen=0.1;
double TraceRay::traceThre=0.01;
TraceRay::TraceRay()
{

}

TraceRay::TraceRay(P a, P b)
{
	ori = a;
	dir = safeNorm(b - a);
}

void TraceRay::Clear(bool keepColor)
{
	for (auto& iter : i)
	{
		auto ti = Cast<TraceRayI*>(iter);
		delete ti;
	}

	for (auto& iter : o)
	{
		auto to = Cast<TraceRayO*>(iter);
		delete to;
	}

	i.clear();
	o.clear();

	bStopTrace = false;
	if (!keepColor)
	{
		color = P(0.0);
	}
	dir = P(0.0);
	ori = P(0.0);
}

void TraceRay::SetMode(rayTraceMode traceMode, rayTraceBounceMode bounceMode)
{
	//Set I
	if (traceMode == rayTraceMode_SDFSphere)
	{
		if (bounceMode == rayTraceBounceMode_MonteCarlo)
		{
			i += new TraceRayI_SDFSphereMonteCarlo(this);
		}
		else
		{
			i += new TraceRayI_SDFSphere(this);
		}
	}

	//Set O
	if (bounceMode == rayTraceBounceMode_cheap)
	{
		o += new TraceRayO_CheapBounce(this);
	}
	else if (bounceMode == rayTraceBounceMode_reflect)
	{
		o += new TraceRayO_ReflectBounce(this);
	}
	else if (bounceMode == rayTraceBounceMode_MonteCarlo)
	{
		o += new TraceRayO_MonteCarlo(this);
	}

}

void TraceRay::Trace(rayTraceWorld* world)
{
	auto tracei = Cast<TraceRayI*>(i[0]);
	tracei->Trace(world);
}

void TraceRay::ShadeAfterHit(rayTraceWorld* world, TraceInfo info)
{
	auto tracei = Cast<TraceRayI*>(i[0]);
	tracei->ShadeAfterHit(world, info);
}

P TraceRay::Ray(double len)
{
	ori = ori + dir * len;
	return ori;
}

void TraceRay::Bounce(const TraceInfo& info)
{
	dir = reflect(dir, info.hitN);
}
//### TraceRay

//### TraceRayI
TraceRayI::TraceRayI(TraceRay* y_):y(y_)
{

}

TraceRayI:: ~TraceRayI()
{

}
//### TraceRayI

//### TraceRayO
TraceRayO::TraceRayO(TraceRay* y_)
	:y(y_)
{

}

TraceRayO::~TraceRayO()
{
	if (matPolicy != nullptr)
	{
		delete matPolicy;
	}
}

void TraceRayO::InitMaterialPolicy(rayTraceMaterialMode matMode)
{

}

void TraceRayO::FinalUnhitGather()
{

}

void TraceRayO::FinalHitGather()
{

}

void TraceRayO::PrepareMaterialExtra(Material& mat)
{

}

//### TraceRayO