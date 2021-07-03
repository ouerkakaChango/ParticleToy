#include "rayTraceOptimizePolicy.h"

#include "rayTraceWorld.h"
#include "rayTraceScreen.h"
#include <iostream>
using std::cout;
using std::endl;

//###rayTraceOptimizePolicy_PerTask
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
	int loopNum = static_cast<int>(ceil((screen->w * screen->h) / double(rayPerTask)));

	if (world->nowBounce == 1)
	{
		str info = "ATTENTION:CPU rayTracing Task ";
		info += loopInx;
		info += " NowTime ";
		info += timer.Record_FormatStr();
		info += " Progress ";
		info += loopInx / (double)loopNum * 100;
		info += "% ...";

		cout << info << endl;
	}

#ifdef RAYTRACE_OpenMP
	omp_set_num_threads(rayPerTask); // 线程个数
	#pragma omp parallel for
#endif
	for (int i = 0; i<rayPerTask; i++)
	{
		auto& ray = screen->optRays[i];

		if (!ray.bStopTrace)
		{
			ray.Trace(world);
		}
	}

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
//###rayTraceOptimizePolicy_PerTask

//### rayTraceOptimizePolicy_NumbaCUDA
void rayTraceOptimizePolicy_NumbaCUDA::InitRequest(rayTraceWorld* world, rayTraceScreen* screen)
{
	traceReq.InitData(world, screen, rayTraceGod.optimizeWorkPath);
}

void rayTraceOptimizePolicy_NumbaCUDA::Trace(rayTraceWorld* world, rayTraceScreen* screen)
{
	str info;
	if (world->nowBounce == 1)
	{
		info += "ATTENTION:CPU rayTracing\n";
	}
	info += "Bounce " + str(world->nowBounce);
	info += " NowTime ";
	info += timer.Record_FormatStr();
	info += " Progress ";
	info += world->nowBounce / (double)world->bounceNum * 100;
	info += "% ...";

	cout << info << endl;

	traceReq.SetRequest(screen);

	//???
	if (world->nowBounce == 2)
	{
		int aa = 1;
	}

	traceReq.SendAndWaitGetResult();
	
	for (int j = 0; j < screen->h; j++)
	{
		for (int i = 0; i < screen->w; i++)
		{
			auto& ray = screen->rays[i][j];
			if (!ray.bStopTrace)
			{
				arr<TraceInfo> infos;
				traceReq.GetResult(infos,i,j);
				ray.ShadeAfterHit(world, infos);
			}
		}
	}


	if (world->nowBounce != world->bounceNum)
	{
		traceReq.PrepareForNext();
	}
}
//### rayTraceOptimizePolicy_NumbaCUDA