#include "TraceRay.h"

#include "rayTraceWorld.h"

//### TraceRay
TraceRay::TraceRay()
{

}

TraceRay::TraceRay(P a, P b)
{
	line.Set(a, b);
	ori = a;
	dir = line.dir();
}

void TraceRay::SetMode(rayTraceMode traceMode, rayTraceBounceMode bounceMode)
{
	//Set I
	if (traceMode == rayTraceMode_SDFSphere)
	{
		i += new TraceRayI_SDFSphere(this);
	}

	//Set O
	if (bounceMode == rayTraceBounceMode_cheap)
	{
		o += new TraceRayO_CheapBounce(this);
	}

	Cast<TraceRayI*>(i[0])->o = Cast<TraceRayO*>(o[0]);
}

TraceInfo TraceRay::Trace(rayTraceWorld* world)
{
	auto tracei = Cast<TraceRayI*>(i[0]);
	return tracei->Trace(world);
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
void TraceRayI::CalculateMaterial(const arr<LightInfo>& lightsInfo, TraceInfo& info)
{
	if (info.obj && info.obj->shape)
	{
		auto mat = info.obj->material;
		if (mat != nullptr)
		{
			o->PrepareMaterialExtra(*mat);
			P n = info.hitN;
			P v = -info.dir;
			info.color = mat->Calculate(lightsInfo, n, v);
			o->matPolicy->UpdateRayAfterCalculate(*y, *mat);
		}
	}
}
//### TraceRayI

//### TraceRayO
void TraceRayO::InitMaterialPolicy(rayTraceMaterialMode matMode)
{

}

void TraceRayO::FinalUnhitGather()
{

}

void TraceRayO::PrepareMaterialExtra(Material& mat)
{

}
//### TraceRayO

//### TraceRayI_SDFSphere
TraceRayI_SDFSphere::TraceRayI_SDFSphere(TraceRay* y_)
{
	TraceRayI::y = y_;
}

TraceInfo TraceRayI_SDFSphere::Trace(rayTraceWorld* world)
{

	TraceInfo info = world->SDF(y->Ray(y->startLen));
	info.dir = y->dir;
	while (info.dis > y->traceThre)
	{
		info = world->SDF(y->Ray(info.dis));
		if (info.dis >= world->maxSDF)
		{
			//没有这个>1的判断也可以，不过省的(0,0,0)和(0,0,0)lerp了。
			if (world->nowBounce > 1)
			{
				//@@@ 第二次Trace不到东西
				y->debugColor = P(0, 1, 0);
				o->FinalUnhitGather();
			}
			y->bStopTrace = true;
			return info;
		}
	}
	info.bHit = true;
	info.dir = y->dir;
	info.hitPos = y->ori;
	info.hitN = info.obj->shape->SDFNormal(y->ori);

	arr<LightInfo> lightsInfo = world->GetLightsInfo(info.hitPos);
	CalculateMaterial(lightsInfo, info);
	o->matPolicy->BlendColor(*y, info);

	//@@@ Trace到东西了
	y->debugColor = P(1, 0, 0);
	if (y->bStopTrace && world->nowBounce == 1)
	{
		//@@@ 第一次Trace到不反射表面
		y->debugColor = P(0, 0, 1);
	}

	return info;
}
//### TraceRayI_SDFSphere

//### TraceRayO_CheapBounce
TraceRayO_CheapBounce::TraceRayO_CheapBounce(TraceRay* y_):
	y(y_)
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
		//!!! 由于只会在PutScreen的时候调用一次，所以字符串比较耗时也不在意
		if (typeStr(*mat.i[0]) == "class BlinnPhongI")
		{
			mat.i += new Extra_BlinnPhongI_CheapBounce;
		}
		//else if (typeStr(*mat.i[0]) == "class PBRI")
		//{
		//	mat.i += new Extra_BlinnPhongI_CheapBounce;
		//}
	}
}

void TraceRayO_CheapBounce::InitMaterialPolicy(rayTraceMaterialMode matMode)
{
	if (matPolicy == nullptr)
	{
		//!!! 由于只会在InitRay的时候调用一次
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

//### rayTraceMaterialPolicy
void rayTraceMaterialPolicy<TraceRayO_CheapBounce, BlinnPhongI>::UpdateRayAfterCalculate(TraceRay& ray, const Material& mat)
{
	auto bounceParam = Cast<Extra_BlinnPhongI_CheapBounce*>(mat.i[1]);
	ray.bStopTrace = zero(bounceParam->reflectness);
}

void rayTraceMaterialPolicy<TraceRayO_CheapBounce, BlinnPhongI>::BlendColor(TraceRay& ray, const TraceInfo& info)
{
	Material& mat = *info.obj->material;
	auto bounceParam = Cast<Extra_BlinnPhongI_CheapBounce*>(mat.i[1]);
	auto cheapBounce = Cast<TraceRayO_CheapBounce*>(ray.o[0]);
	ray.color = lerp(ray.color, info.color, cheapBounce->lastReflectness);
	cheapBounce->lastReflectness *= bounceParam->reflectness;
}

void rayTraceMaterialPolicy<TraceRayO_CheapBounce, PBRI>::BlendColor(TraceRay& ray, const TraceInfo& info)
{
	ray.color = info.color;
}
//### rayTraceMaterialPolicy