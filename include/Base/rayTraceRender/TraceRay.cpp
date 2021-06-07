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
	line.Set(a, b);
	ori = a;
	dir = line.dir();
}

void TraceRay::Clear()
{
	for (auto& ti : i)
	{
		delete ti;
	}

	for (auto& to : o)
	{
		delete to;
	}
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

	Cast<TraceRayI*>(i[0])->o = Cast<TraceRayO*>(o[0]);
}

void TraceRay::Trace(rayTraceWorld* world)
{
	auto tracei = Cast<TraceRayI*>(i[0]);
	tracei->Trace(world);
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
//### TraceRayI

//### TraceRayO
TraceRayO::TraceRayO(TraceRay* y_)
	:y(y_)
{

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

void TraceRayO::CalculateMaterial(rayTraceWorld* world, TraceInfo& info)
{
	auto lightsInfo = world->GetLightsInfo(info.hitPos);
	if (info.obj && info.obj->material)
	{
		auto mat = info.obj->material;
		PrepareMaterialExtra(*mat);
		P n = info.hitN;
		P v = -info.dir;
		info.color = mat->Calculate(lightsInfo, n, v);
		matPolicy->UpdateRayAfterCalculate(*y, *mat);
	}
}
//### TraceRayO