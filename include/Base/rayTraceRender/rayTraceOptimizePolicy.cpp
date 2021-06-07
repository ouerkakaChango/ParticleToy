#include "rayTraceOptimizePolicy.h"

#include "rayTraceWorld.h"
#include "rayTraceScreen.h"
#include <iostream>
using std::cout;
using std::endl;

void rayTraceOptimizePolicy_PerTask::InitTaskRays(rayTraceWorld* world, rayTraceScreen* screen, int loopInx)
{
	if (rayPerTask == 0)
	{
		abort();
	}
	//屏幕x属于[-1,1]，y根据分辨率变化的screen，eye在(0,1,0)对每个像素中心发射ray(trace束)
	double dx = 2.0 / screen->w;
	double ylen = (2.0 * screen->h) / screen->w;

	P startPos = P(-1.0, -ylen / 2, 0.0) + 0.5*P(dx, dx, 0.0);
	for(unsigned int inx = loopInx * rayPerTask;inx < min((loopInx+1)*rayPerTask,screen->w*screen->h);inx++)
	{
		int rayInx = inx - loopInx * rayPerTask;
		auto j = inx / screen->w;
		auto i = inx - j * screen->w;
		P endPos = startPos + P(i*dx, j*dx, 0.0);
		{
			screen->optRays[rayInx] = TraceRay(screen->pos + P(0, 0, 1), screen->pos + endPos);
			screen->optRays[rayInx].SetMode(world->traceMode, world->bounceMode);
			Cast<TraceRayO*>(screen->optRays[rayInx].o[0])->InitMaterialPolicy(world->matMode);
		}
	}
}

void rayTraceOptimizePolicy_PerTask::Trace(rayTraceWorld* world, rayTraceScreen* screen, int loopInx)
{
	if (world->nowBounce == 1)
	{
		str info = "ATTENTION:CPU rayTracing Task ";
		info += loopInx;
		info += " ...";
		cout << info << endl;
	}
	for (int i = 0; i<rayPerTask; i++)
	{
		auto& ray = screen->optRays[i];

		if (!ray.bStopTrace)
		{
			ray.Trace(world);
		}
	}

	int loopNum = ceil((screen->w * screen->h) / double(rayPerTask));
	if (loopInx == loopNum - 1 && world->nowBounce == world->bounceNum)
	{
		cout << "ATTENTION:CPU rayTracing All Task done." << endl;
	}
}

void rayTraceOptimizePolicy_PerTask::FinalGather(rayTraceScreen* screen, int loopInx)
{
	for (unsigned int inx = loopInx * rayPerTask; inx < min((loopInx + 1)*rayPerTask, screen->w*screen->h); inx++)
	{
		int rayInx = inx - loopInx * rayPerTask;
		auto j = inx / screen->w;
		auto i = inx - j * screen->w;
		screen->colorBuffer[i][j] = screen->optRays[rayInx].color;
	}
}

void rayTraceOptimizePolicy_PerTask::Clear(rayTraceScreen* screen)
{
	for (auto&ray : screen->optRays)
	{
		ray.Clear();
	}
}