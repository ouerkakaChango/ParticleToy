#include "TraceRayO_MonteCarlo.h"

#include "../../TraceMode/SDFSphereTrace/TraceRayI_SDFSphereMonteCarlo.h"
#include "../ReflectBounce/TraceRayO_ReflectBounce.h"
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
		//����ʲô�������ͣ���MonteCarlo�е�Policy��һ��
		matPolicy = new rayTraceMaterialPolicy<TraceRayO_MonteCarlo, MaterialI>;
	}
}

void TraceRayO_MonteCarlo::FinalHitGather()
{
	//https://blog.csdn.net/weixin_44176696/article/details/113418991
	auto& subRays = Cast<TraceRayI_SDFSphereMonteCarlo*>(y->i[0])->subRays;
	if (subRays.size() == 0)
	{
		y->color = shadeTask.Calculate(); 
		return;
	}
	ShadeTask indirTask = shadeTask;
	indirTask.lightInfos.clear();
	indirTask.control = new MaterialExtraControl;
	indirTask.control->bIgnoreEmissive = true;
	int validCount = 0;
	for (int i = 0; i < subRays.size(); i++)
	{
		auto& ray = subRays[i];
		auto to = Cast<TraceRayO_ReflectBounce*>(ray.o[0]);
		if (to->traceInfos.size() > 0)
		{
			P lpos = to->traceInfos[0].hitPos;
			P pos = shadeTask.p;
			PointLight tLight(lpos, ray.color);
			indirTask.lightInfos += tLight.GetLightInfo(pos, 1.0);
			validCount++;
		}
	}
	if (validCount == 0)
	{
		y->color = shadeTask.Calculate();
		return;
	}
	P indirColor = indirTask.Calculate()* 2.0 * PI / validCount;
	y->color = shadeTask.Calculate() + indirColor;
	y->Clear();
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