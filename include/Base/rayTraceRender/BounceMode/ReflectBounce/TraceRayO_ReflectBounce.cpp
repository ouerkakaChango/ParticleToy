#include "TraceRayO_ReflectBounce.h"

#include "../../rayTraceMaterialExtra.h"
#include "../../rayTraceWorld.h"
#include "Object.h"

//### TraceRayO_ReflectBounce
TraceRayO_ReflectBounce::TraceRayO_ReflectBounce(TraceRay* y_)
	:TraceRayO(y_)
{

}

TraceRayO_ReflectBounce::~TraceRayO_ReflectBounce()
{
	int aaa=1;
}

void TraceRayO_ReflectBounce::FinalUnhitGather()
{
	ReflectBounceGather();
}

void TraceRayO_ReflectBounce::FinalHitGather()
{
	ReflectBounceGather();
}

void TraceRayO_ReflectBounce::ReflectBounceGather()
{
	if (shadeTasks.size() == 0)
	{
		return;
	}
	for (int inx = shadeTasks.size() - 1; inx >= 1; inx--)
	{
		P indirectLightColor = shadeTasks[inx].Calculate();

		PointLight tLight(shadeTasks[inx].p, indirectLightColor);
		shadeTasks[inx - 1].lightInfos += tLight.GetLightInfo(shadeTasks[inx - 1].p, reflectKs[inx]);
	}
	y->color = shadeTasks[0].Calculate();
}

void TraceRayO_ReflectBounce::PrepareMaterialExtra(Material& mat)
{
#ifdef  RAYTRACE_OpenMP
	rayTraceGod.mtx.lock();
#endif 

	if (mat.i.size() == 1)
	{
		//不论什么材质类型，在ReflectBounce中的MatExtra都一样
		mat.i += new Extra_ReflectBounce;
	}
#ifdef  RAYTRACE_OpenMP
	rayTraceGod.mtx.unlock();
#endif 
}

void TraceRayO_ReflectBounce::InitMaterialPolicy(rayTraceMaterialMode matMode)
{
	if (matPolicy == nullptr)
	{
		//不论什么材质类型，在ReflectBounce中的Policy都一样
		matPolicy = new rayTraceMaterialPolicy<TraceRayO_ReflectBounce, MaterialI>;
	}
}

void TraceRayO_ReflectBounce::ResetData()
{
	reflectKs.clear();
	shadeTasks.clear();
	traceInfos.clear();
}
//### TraceRayO_ReflectBounce

//### rayTraceMaterialPolicy<TraceRayO_ReflectBounce, MaterialI>
void rayTraceMaterialPolicy<TraceRayO_ReflectBounce, MaterialI>::BlendColor(rayTraceWorld* world, TraceRay& ray, const TraceInfo& info)
{
	auto reflectO = Cast<TraceRayO_ReflectBounce*>(ray.o[0]);

	if (info.obj && info.obj->material)
	{
		auto mat = info.obj->material;
		reflectO->PrepareMaterialExtra(*mat);
		
		auto bounceParam = Cast<Extra_ReflectBounce*>(mat->i[1]);
		reflectO->reflectKs += bounceParam->reflectEnegyRate;
	}

	reflectO->traceInfos += info;

	if (info.obj && info.obj->material)
	{
		auto mat = info.obj->material;
		P n = info.hitN;
		P v = -info.dir;
		auto lightsInfo = world->GetLightsInfo(info.hitPos);
		reflectO->shadeTasks += ShadeTask(mat, info.hitPos, n, v, lightsInfo);
	}
}
//### rayTraceMaterialPolicy<TraceRayO_ReflectBounce, MaterialI>