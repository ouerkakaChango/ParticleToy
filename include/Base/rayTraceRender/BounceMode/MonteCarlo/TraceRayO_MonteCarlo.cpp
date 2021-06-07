#include "TraceRayO_MonteCarlo.h"

#include "Object.h"
#include "../../rayTraceWorld.h"
//### TraceRayO_MonteCarlo
TraceRayO_MonteCarlo::TraceRayO_MonteCarlo(TraceRay* y_)
	:TraceRayO(y_)
{

}

void TraceRayO_MonteCarlo::CalculateMaterial(rayTraceWorld* world, TraceInfo& info)
{
	//if (info.obj && info.obj->material)
	//{
		//auto mat = info.obj->material;
		//PrepareMaterialExtra(*mat);
		//matPolicy->UpdateRayAfterCalculate(*y, *mat);
	//}
}


void TraceRayO_MonteCarlo::InitMaterialPolicy(rayTraceMaterialMode matMode)
{
	if (matPolicy == nullptr)
	{
		//不论什么材质类型，在MonteCarlo中的Policy都一样
		matPolicy = new rayTraceMaterialPolicy<TraceRayO_MonteCarlo, MaterialI>;
	}
}

void TraceRayO_MonteCarlo::FinalHitGather()
{
	y->color = shadeTask.Calculate();
}
//### TraceRayO_MonteCarlo

//### rayTraceMaterialPolicy<TraceRayO_MonteCarlo, MaterialI>
void rayTraceMaterialPolicy<TraceRayO_MonteCarlo, MaterialI>::BlendColor(rayTraceWorld* world, TraceRay& ray, const TraceInfo& info)
{
	auto reflectO = Cast<TraceRayO_MonteCarlo*>(ray.o[0]);

	if (info.obj && info.obj->material)
	{
		auto mat = info.obj->material;
		P n = info.hitN;
		P v = -info.dir;
		auto lightsInfo = world->GetLightsInfo(info.hitPos);
		reflectO->shadeTask = ShadeTask(mat, info.hitPos, n, v, lightsInfo);
	}
}
//### rayTraceMaterialPolicy<TraceRayO_MonteCarlo, MaterialI>