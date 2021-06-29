#include "RTTraceRequest.h"

#include <fstream>
#include "../TraceRay.h"
#include "../rayTraceWorld.h"
#include "../rayTraceScreen.h"
#include "../TraceMode/SDFSphereTrace/TraceRayI_SDFSphereMonteCarlo.h"

using std::endl;

//### RTTraceRequest
RTTraceRequest::RTTraceRequest()
{
	
}

void RTTraceRequest::InitData(rayTraceWorld* world, rayTraceScreen* screen, const str& outPath, RequestMode mode_)
{
	mode = mode_;
	if (world->traceMode == rayTraceMode_SDFSphere)
	{
		auto to = new RTTraceRequestO_SDFSphere(this);
		to->InitData(world,screen);
		o += to;
	}
	if (mode == RequestMode_txt)
	{
		auto ti = new RTTraceRequestI_txt;
		ti->outPath = outPath;
		ti->o = Cast<RTTraceRequestO*>(o[0]);
		i += ti;
	}
}

void RTTraceRequest::SetRequest(rayTraceScreen* screen_)
{
	screen = screen_;
}

void RTTraceRequest::PrepareForNext()
{
	auto data = Cast<RTTraceRequestO*>(o[0]);
	data->PrepareForNext();
}

void RTTraceRequest::SendAndWaitGetResult()
{
	//???
	auto ti = Cast<RTTraceRequestI*>(i[0]);
	for (int reqInx = 0; reqInx < requestNum;reqInx++ )
	{
		ti->SendSingleRequest(reqInx);
		ti->CallCalculation();
		ti->WaitForResult(reqInx);
	}
}
//### RTTraceRequest

//### RTTraceRequestO
RTTraceRequestO::RTTraceRequestO(RTTraceRequest* y_):y(y_)
{

}
//### RTTraceRequestO

//### RTTraceRequestO_SDFSphere
RTTraceRequestO_SDFSphere::RTTraceRequestO_SDFSphere(RTTraceRequest* y_):
	RTTraceRequestO(y_)
{

}
void RTTraceRequestO_SDFSphere::InitData(rayTraceWorld* world_, rayTraceScreen* screen)
{
	world = world_;
	w = screen->w;
	h = screen->h;

	auto& ray = screen->rays[0][0];
	auto rayi = Cast<TraceRayI*>(ray.i[0]);

	if (typeStr(*rayi) == "class TraceRayI_SDFSphereMonteCarlo")
	{
		y->requestNum = 1 + TraceRayI_SDFSphereMonteCarlo::spp * (world->bounceNum-1);
	}
	else
	{
		abort();
	}
}

void RTTraceRequestO_SDFSphere::PrepareForNext()
{

}

//### RTTraceRequestO_SDFSphere

//### RTTraceRequestI_txt
void RTTraceRequestI_txt::SendRequest()
{
	if (typeStr(*o) == "class RTTraceRequestO_SDFSphere")
	{
		abort();
	}
	else
	{
		abort();
	}
}

void RTTraceRequestI_txt::SendSingleRequest(int reqInx)
{
	if (typeStr(*o) == "class RTTraceRequestO_SDFSphere")
	{
		auto to = Cast<RTTraceRequestO_SDFSphere*>(o);
		str tFilePath = outPath + "\\traceReq" + str(reqInx)+".txt";
		std::ofstream f(tFilePath.data, std::ios::out);
		f << to->w << " " << to->h << " " << endl;
		if (reqInx == 0)
		{
			for (int j = 0; j < to->h; j++)
			{
				for (int i = 0; i < to->w; i++)
				{
					auto& ray = o->y->screen->rays[i][j];
					f << ray.ori.ToStr() << " " << ray.dir.ToStr() << endl;
				}
			}
		}
		else
		{
			int subRayInx = reqInx % TraceRayI_SDFSphereMonteCarlo::spp - 1;
			for (int j = 0; j < to->h; j++)
			{
				for (int i = 0; i < to->w; i++)
				{
					auto& ray = o->y->screen->rays[i][j];
					auto subRay = Cast<TraceRayI_SDFSphereMonteCarlo*>(ray.i[0])->subRays[subRayInx];
					f << subRay.ori.ToStr() << " " << subRay.dir.ToStr() << endl;
				}
			}
		}
		f.close();
	}
	else
	{
		abort();
	}
}

void RTTraceRequestI_txt::CallCalculation()
{

}

void RTTraceRequestI_txt::WaitForResult(int reqInx)
{
	//wait check result file is exist
}
//### RTTraceRequestI_txt