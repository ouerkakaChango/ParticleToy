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

void rayTraceWorld::SetTraceSettings(int bounceNum_, 
	rayTraceMode traceMode_, 
	rayTraceBounceMode bounceMode_, 
	rayTraceMaterialMode matMode_)
{
	bounceNum = bounceNum_;
	traceMode = traceMode_;
	bounceMode = bounceMode_;
	matMode = matMode_;
}

void rayTraceWorld::PutScreen(rayTraceScreen* screen)
{
	//!!! 多screen正确性还没考虑
	if (optimizeMode == rayTraceOptimizeMode_None)
	{
		screen->InitRays(traceMode, bounceMode, matMode);
		TraceRay& ray = screen->rays[0][0];
		for (auto& obj : objs)
		{
			Cast<TraceRayO*>(ray.o[0])->PrepareMaterialExtra(*obj->material);
		}
	}
	else if (optimizeMode == rayTraceOptimizeMode_PerTask)
	{
		screen->InitBuffers();
	}
	else if (optimizeMode == rayTraceOptimizeMode_NumbaCUDA)
	{
		screen->InitRays(traceMode, bounceMode, matMode);
		TraceRay& ray = screen->rays[0][0];
		for (auto& obj : objs)
		{
			Cast<TraceRayO*>(ray.o[0])->PrepareMaterialExtra(*obj->material);
		}
	}
	screens += screen;
}

void rayTraceWorld::Evolve()
{
	if (optimizeMode == rayTraceOptimizeMode_None)
	{
		for (int i = 0; i < bounceNum; i++)
		{
			for (auto& screen : screens)
			{
				nowBounce += 1;
				screen->Trace(this);
			}
		}
		for (auto& screen : screens)
		{
			screen->FinalGather();
		}
	}
	else if (optimizeMode == rayTraceOptimizeMode_PerTask)
	{
		auto opt = Cast<rayTraceOptimizePolicy_PerTask*>(optimizePolicy);
		auto rayPerTask = opt->rayPerTask;
		for (auto& screen : screens)
		{
			screen->optRays.resize(rayPerTask);
			int loopNum = static_cast<int>(ceil((screen->w * screen->h) / double(rayPerTask)));
			opt->timer.Start();
			for (int loopInx = 0; loopInx < loopNum; loopInx++)
			{
				opt->InitTaskRays(this, screen, loopInx);
				for (int i = 0; i < bounceNum; i++)
				{
					nowBounce += 1;
					opt->Trace(this, screen, loopInx);
				}
				opt->FinalGather(screen, loopInx);
				nowBounce = 0;
				opt->Clear(screen);
			}
		}
	}
	else if (optimizeMode == rayTraceOptimizeMode_NumbaCUDA)
	{
		auto opt = Cast<rayTraceOptimizePolicy_NumbaCUDA*>(optimizePolicy);
		for (auto& screen : screens)
		{
			opt->InitRequest(this, screen);
			for (nowBounce=1; nowBounce <= bounceNum; nowBounce++)
			{
				opt->Trace(this, screen);
			}
		}

		for (auto& screen : screens)
		{
			screen->FinalGather();
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

arr<LightInfo> rayTraceWorld::GetLightsInfo(const P& pos)
{
	arr<LightInfo> re;
	for (auto& light : lights)
	{
		re += light->GetLightInfo(pos);
	}
	return re;
}

void rayTraceWorld::SetOptimizeMode(rayTraceOptimizeMode optimizeMode_)
{
	optimizeMode = optimizeMode_;
	if (optimizeMode == rayTraceOptimizeMode_PerTask)
	{
		optimizePolicy = new rayTraceOptimizePolicy_PerTask;
	}
	else if (optimizeMode == rayTraceOptimizeMode_NumbaCUDA)
	{
		optimizePolicy = new rayTraceOptimizePolicy_NumbaCUDA;
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

void rayTraceWorldR::AddMaterial(Object* obj)
{
	obj->material = new Material;
	if (y->matMode == rayTraceMaterialMode_BlinnPhong)
	{
		obj->material->i += new BlinnPhongI;
		obj->material->o += new BlinnPhongO;
	}
	else if (y->matMode == rayTraceMaterialMode_PBR)
	{
		obj->material->i += new PBRI;
		obj->material->o += new PBRO;
	}
}

Object* rayTraceWorldR::PutShape(Shape* shape, const str& name)
{
	auto obj = new Object;
	AddMaterial(obj);
	obj->SetShape(shape);
	obj->name = name;
	y->objs += obj;
	return obj;
}

void rayTraceWorldR::PutScreen(rayTraceScreen* screen)
{
	y->PutScreen(screen);
}

void rayTraceWorldR::PutLight(Light* light)
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
	f.close();
	std::cout << "ATTENTION: CPU Writing FrameBuffer done\n";
}
//### rayTraceWorldR