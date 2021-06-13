#include "TraceRayO_CheapBounce.h"

#include "../../rayTraceWorld.h"
#include "../../rayTraceMaterialExtra.h"
#include "Object.h"

//### TraceRayO_CheapBounce
TraceRayO_CheapBounce::TraceRayO_CheapBounce(TraceRay* y_) :
	TraceRayO(y_)
{

}

void TraceRayO_CheapBounce::FinalUnhitGather()
{
	y->color = lerp(y->color, P(0, 0, 0), lastReflectness);
}

void TraceRayO_CheapBounce::PrepareMaterialExtra(Material& mat)
{
	if (mat.i.size() == 1)
	{
		//由于只会在PutScreen的时候调用一次，所以字符串比较耗时也不在意
		if (typeStr(*mat.i[0]) == "class BlinnPhongI")
		{
			mat.i += new Extra_BlinnPhongI_CheapBounce;
		}
	}
}

void TraceRayO_CheapBounce::InitMaterialPolicy(rayTraceMaterialMode matMode)
{
	if (matPolicy == nullptr)
	{
		if (matMode == rayTraceMaterialMode_BlinnPhong)
		{
			matPolicy = new rayTraceMaterialPolicy<TraceRayO_CheapBounce, BlinnPhongI>;
		}
		else if (matMode == rayTraceMaterialMode_PBR)
		{
			matPolicy = new rayTraceMaterialPolicy<TraceRayO_CheapBounce, PBRI>;
		}
	}
}
//### TraceRayO_CheapBounce

//### rayTraceMaterialPolicy<TraceRayO_CheapBounce, BlinnPhongI>
void rayTraceMaterialPolicy<TraceRayO_CheapBounce, BlinnPhongI>::UpdateRayAfterCalculate(TraceRay& ray, const Material& mat)
{
	auto bounceParam = Cast<Extra_BlinnPhongI_CheapBounce*>(mat.i[1]);
	ray.bStopTrace = zero(bounceParam->reflectness);
}

void rayTraceMaterialPolicy<TraceRayO_CheapBounce, BlinnPhongI>::BlendColor(rayTraceWorld* world, TraceRay& ray, const TraceInfo& info)
{
	auto cheapBounceO = Cast<TraceRayO_CheapBounce*>(ray.o[0]);

	P nowColor;
	auto lightsInfo = world->GetLightsInfo(info.hitPos);
	if (info.obj && info.obj->material)
	{
		auto mat = info.obj->material;
		cheapBounceO->PrepareMaterialExtra(*mat);
		P n = info.hitN;
		P v = -info.dir;
		nowColor = mat->Calculate(lightsInfo, n, v);
		UpdateRayAfterCalculate(ray, *mat);
	}

	Material& mat = *info.obj->material;
	auto bounceParam = Cast<Extra_BlinnPhongI_CheapBounce*>(mat.i[1]);
	ray.color = lerp(ray.color, nowColor, cheapBounceO->lastReflectness);
	cheapBounceO->lastReflectness *= bounceParam->reflectness;
}
//### rayTraceMaterialPolicy<TraceRayO_CheapBounce, BlinnPhongI>