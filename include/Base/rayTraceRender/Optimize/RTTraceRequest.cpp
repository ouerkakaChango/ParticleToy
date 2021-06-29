#include "RTTraceRequest.h"

#include "../TraceRay.h"
#include "../rayTraceWorld.h"
#include "../rayTraceScreen.h"
#include "../TraceMode/SDFSphereTrace/TraceRayI_SDFSphereMonteCarlo.h"

//### RTTraceRequest
RTTraceRequest::RTTraceRequest()
{
	
}

void RTTraceRequest::InitData(rayTraceWorld* world, rayTraceScreen* screen)
{
	if (world->traceMode == rayTraceMode_SDFSphere)
	{
		auto to = new RTTraceRequestData_SDFSphere;
		to->InitData(world,screen);
		o += to;
	}
}

void RTTraceRequest::SetRequest(int i, int j, TraceRay& ray)
{
	auto data = Cast<RTTraceRequestData*>(o[0]);
	data->SetRequest(i, j, ray);
}

void RTTraceRequest::PrepareForNext()
{
	auto data = Cast<RTTraceRequestData*>(o[0]);
	data->PrepareForNext();
}
//### RTTraceRequest

//### RTTraceRequestData_SDFSphere
void RTTraceRequestData_SDFSphere::InitData(rayTraceWorld* world_, rayTraceScreen* screen)
{
	world = world_;
	w = screen->w;
	h = screen->h;

	auto& ray = screen->rays[0][0];
	auto ti = Cast<TraceRayI*>(ray.i[0]);

	if (typeStr(*ti) == "class TraceRayI_SDFSphereMonteCarlo")
	{
		auto tti = Cast<TraceRayI_SDFSphereMonteCarlo*>(ti);
		data.resize(1);
		data[0].resize(w, h);
	}
	else
	{
		abort();
	}
}

void RTTraceRequestData_SDFSphere::PrepareForNext()
{
	if (world->traceMode == rayTraceMode_SDFSphere && world->bounceMode == rayTraceBounceMode_MonteCarlo)
	{
		data.resize(TraceRayI_SDFSphereMonteCarlo::spp);
		for (auto& page : data)
		{
			page.resize(w, h);
		}
	}
	else
	{
		abort();
	}
}

void RTTraceRequestData_SDFSphere::SetRequest(int i, int j, TraceRay& ray)
{
	for (auto&page : data)
	{
		auto& req = page[i][j];
		req.ori = ray.ori;
		req.dir = ray.dir;
	}
}
//### RTTraceRequestData_SDFSphere