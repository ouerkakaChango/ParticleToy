#include "rayTraceWorld.h"

#include "Render/Material.h"
#include <iostream>
#include <fstream>
using std::cout;
using std::endl;

//### TraceTime
TraceTime::TraceTime()
{

}
//### TraceTime

//### rayTraceWorld
rayTraceWorld::rayTraceWorld()
{

	//Set R
	r += new rayTraceWorldR(this);
}

void rayTraceWorld::SetTraceSettings(int bounceNum_, rayTraceBounceMode bounceMode_)
{
	bounceNum = bounceNum_;
	bounceMode = bounceMode_;
}

void rayTraceWorld::Evolve()
{
	for (int i = 0; i < bounceNum; i++)
	{
		for (auto& screen : screens)
		{
			nowBounce += 1;
			screen->Trace(this);
		}
	}
}

TraceInfo rayTraceWorld::SDF(P pos)
{
	//!!!
	TraceInfo re;
	re.dis = maxSDF;
	if (objs.size() == 0)
	{
		return re;
	}
	double minDis = maxSDF;
	Object* tObj = nullptr;
	for (auto& obj : objs)
	{
		if (obj->shape != nullptr)
		{
			double tDis = obj->shape->SDF(pos);
			if (tDis < minDis)
			{
				minDis = tDis;
				tObj = obj;
			}
		}
	}
	re.dis = minDis;
	re.obj = tObj;
	return re;
}

void rayTraceWorld::PolicyPrepareMaterialForTrace(Material& mat)
{
	if (typeStr(*mat.i[0]) == "class BlinnPhongI")
	{
		if (bounceMode == rayTraceBounceMode_cheap)
		{
			if (mat.i.size() == 1)
			{
				mat.i += new Extra_BlinnPhongI_CheapBounce;
			}
		}
	}
}

void rayTraceWorld::PolicyUpdateRayAfterCalculate(TraceRay& ray, const Material& mat)
{
	if (typeStr(*mat.i[0]) == "class BlinnPhongI")
	{
		if (bounceMode == rayTraceBounceMode_cheap)
		{
			auto bounceParam = Cast<Extra_BlinnPhongI_CheapBounce*>(mat.i[1]);
			ray.bStopTrace = zero(bounceParam->reflectness);
		}
	}
}

void rayTraceWorld::CalculateMaterial(TraceRay& ray, TraceInfo& info)
{
	if (info.obj && info.obj->shape)
	{
		auto mat = info.obj->material;
		if (mat != nullptr)
		{
			
			PolicyPrepareMaterialForTrace(*mat);
			P n = info.hitN;
			P v = -info.dir;
			info.color = mat->Calculate(lights, n, v);
			PolicyUpdateRayAfterCalculate(ray,*mat);
			
		}
	}
}

void rayTraceWorld::PolicyBlendColor(TraceRay& ray, const Material& mat, const TraceInfo& info)
{
	if (typeStr(*mat.i[0]) == "class BlinnPhongI")
	{
		if (bounceMode == rayTraceBounceMode_cheap)
		{
			auto bounceParam = Cast<Extra_BlinnPhongI_CheapBounce*>(mat.i[1]);
			ray.color = lerp(ray.color, info.color, ray.lastReflectness);
			ray.lastReflectness *= bounceParam->reflectness;
		}
	}
}

void rayTraceWorld::BlendColor(TraceRay& ray, const TraceInfo& info)
{
	if (info.obj && info.obj->shape)
	{
		auto mat = info.obj->material;
		if (mat != nullptr)
		{
			PolicyBlendColor(ray, *mat, info);
		}
	}
}
//### rayTraceWorld

//### rayTraceWorldR
void rayTraceWorldR::SayI()
{
	for (auto& obj : y->objs)
	{
		cout << obj->name << endl;
	}
}

Object* rayTraceWorldR::PutShape(Shape* shape, const str& name)
{
	auto obj = new Object;
	obj->SetShape(shape);
	obj->name = name;
	y->PolicyPrepareMaterialForTrace(*obj->material);
	y->objs += obj;
	return obj;
}

void rayTraceWorldR::PutScreen(rayTraceScreen* screen)
{
	y->screens += screen;
}

void rayTraceWorldR::PutLight(DirectionalLight* light)
{
	y->lights += light;
}

void rayTraceWorldR::Evolve()
{
	y->Evolve();
}

void rayTraceWorldR::SaveScreenBufferFrame(rayTraceScreen* screen, const str& bufferName, const str& filePath)
{
	std::cout << "ATTENTION: CPU Writing FrameBuffer...\n";
	std::ofstream f(filePath.data, std::ios::out);
	f << screen->w << " " << screen->h << endl;
	for (int j = 0; j < screen->h; j++)
	{
		for (int i = 0; i < screen->w; i++)
		{
			P pixel;
			if (bufferName == "debug")
			{
				pixel = screen->debugFrameBuffer[i][j];
			}
			else if (bufferName == "color")
			{
				pixel = screen->colorBuffer[i][j];
			}
			else if (bufferName == "normal")
			{
				pixel = screen->normalBuffer[i][j];
			}
			else if (bufferName == "pos")
			{
				pixel = screen->posBuffer[i][j];
			}
			int R = Cast<int>(pixel.x * 255);
			int G = Cast<int>(pixel.y * 255);
			int B = Cast<int>(pixel.z * 255);
			f << R << " " << G << " " << B << endl;
		}
	}
	std::cout << "ATTENTION: CPU Writing FrameBuffer done\n";
}
//### rayTraceWorldR