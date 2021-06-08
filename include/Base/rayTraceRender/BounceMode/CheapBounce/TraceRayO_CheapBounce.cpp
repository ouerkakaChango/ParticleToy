#include "TraceRayO_CheapBounce.h"

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
		//����ֻ����PutScreen��ʱ�����һ�Σ������ַ����ȽϺ�ʱҲ������
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
	Material& mat = *info.obj->material;
	auto bounceParam = Cast<Extra_BlinnPhongI_CheapBounce*>(mat.i[1]);
	auto cheapBounce = Cast<TraceRayO_CheapBounce*>(ray.o[0]);
	ray.color = lerp(ray.color, info.color, cheapBounce->lastReflectness);
	cheapBounce->lastReflectness *= bounceParam->reflectness;
}
//### rayTraceMaterialPolicy<TraceRayO_CheapBounce, BlinnPhongI>