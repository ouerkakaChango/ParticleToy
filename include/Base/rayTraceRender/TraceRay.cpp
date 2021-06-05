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
	else if (bounceMode == rayTraceBounceMode_reflect)
	{
		o += new TraceRayO_ReflectBounce(this);
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

//### TraceRayI_SDFSphere
TraceRayI_SDFSphere::TraceRayI_SDFSphere(TraceRay* y_)
{
	TraceRayI::y = y_;
}

void TraceRayI_SDFSphere::Trace(rayTraceWorld* world)
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
			return;
		}
	}
	info.bHit = true;
	info.dir = y->dir;
	info.hitPos = y->ori;
	info.hitN = info.obj->shape->SDFNormal(y->ori);

	arr<LightInfo> lightsInfo = world->GetLightsInfo(info.hitPos);
	o->CalculateMaterial(world, info);
	o->matPolicy->BlendColor(world, *y, info);

	//{
	//	//@@@ Trace到东西了
	//	y->debugColor = P(1, 0, 0);
	//	if (y->bStopTrace && world->nowBounce == 1)
	//	{
	//		//@@@ 第一次Trace到不反射表面
	//		y->debugColor = P(0, 0, 1);
	//	}
	//}

	if (world->nowBounce == world->bounceNum)
	{
		y->bStopTrace = true;
		o->FinalHitGather();
	}
	if (world->nowBounce < world->bounceNum)
	{
		y->Bounce(info);
	}
	return;
}
//### TraceRayI_SDFSphere

//### TraceRayO_CheapBounce
TraceRayO_CheapBounce::TraceRayO_CheapBounce(TraceRay* y_):
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

//### TraceRayO_ReflectBounce
TraceRayO_ReflectBounce::TraceRayO_ReflectBounce(TraceRay* y_)
	:TraceRayO(y_)
{

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
	for (int inx = shadeTasks.size()-1; inx >= 1; inx--)
	{
		P indirectLightColor = shadeTasks[inx].Calculate();

		PointLight tLight(shadeTasks[inx].p, indirectLightColor);
		shadeTasks[inx - 1].lightInfos += tLight.GetLightInfo(shadeTasks[inx-1].p, reflectKs[inx]);
	}
	y->color = shadeTasks[0].Calculate();
}

void TraceRayO_ReflectBounce::PrepareMaterialExtra(Material& mat)
{
	if (mat.i.size() == 1)
	{
		//不论什么材质类型，在ReflectBounce中的MatExtra都一样
		mat.i += new Extra_ReflectBounce;
	}
}

void TraceRayO_ReflectBounce::InitMaterialPolicy(rayTraceMaterialMode matMode)
{
	if (matPolicy == nullptr)
	{
		//不论什么材质类型，在ReflectBounce中的Policy都一样
		matPolicy = new rayTraceMaterialPolicy<TraceRayO_ReflectBounce, MaterialI>;
	}
}

void TraceRayO_ReflectBounce::CalculateMaterial(rayTraceWorld* world, TraceInfo& info)
{
	if (info.obj && info.obj->material)
	{
		auto mat = info.obj->material;
		PrepareMaterialExtra(*mat);
		matPolicy->UpdateRayAfterCalculate(*y, *mat);
	}
}
//### TraceRayO_ReflectBounce

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

//### rayTraceMaterialPolicy<TraceRayO_ReflectBounce, MaterialI>
void rayTraceMaterialPolicy<TraceRayO_ReflectBounce, MaterialI>::UpdateRayAfterCalculate(TraceRay& ray, const Material& mat)
{
	auto bounceParam = Cast<Extra_ReflectBounce*>(mat.i[1]);
	auto reflectO = Cast<TraceRayO_ReflectBounce*>(ray.o[0]);
	reflectO->reflectKs += bounceParam->reflectEnegyRate;
}

void rayTraceMaterialPolicy<TraceRayO_ReflectBounce, MaterialI>::BlendColor(rayTraceWorld* world, TraceRay& ray, const TraceInfo& info)
{
	auto reflectO = Cast<TraceRayO_ReflectBounce*>(ray.o[0]);
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