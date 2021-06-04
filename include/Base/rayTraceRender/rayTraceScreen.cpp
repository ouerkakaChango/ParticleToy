#include "rayTraceScreen.h"

#include "rayTraceWorld.h"
#include <iostream>
using std::cout;
using std::endl;

rayTraceScreen::rayTraceScreen(int w_, int h_):
	w(w_),h(h_)
{
	//InitRays();
}

void rayTraceScreen::InitRays(rayTraceMode traceMode, rayTraceBounceMode bounceMode, rayTraceMaterialMode matMode)
{
	rays.resize(w, h);
	debugFrameBuffer.resize(w, h);
	colorBuffer.resize(w, h);
	normalBuffer.resize(w, h);
	posBuffer.resize(w, h);
	//屏幕x属于[-1,1]，y根据分辨率变化的screen，eye在(0,1,0)对每个像素中心发射ray(trace束)
	double dx = 2.0 / w;
	double ylen = (2.0*h) / w;
	
	P startPos = P(-1.0, -ylen/2, 0.0) + 0.5*P(dx, dx, 0.0);
	for (int j = 0; j < h; j++)
	{
		for (int i = 0; i < w; i++)
		{
			P endPos = startPos + P(i*dx,j*dx,0.0);
			rays[i][j] = TraceRay(P(0, 0, 1), endPos); 
			rays[i][j].SetMode(traceMode, bounceMode);
			Cast<TraceRayO*>(rays[i][j].o[0])->InitMaterialPolicy(matMode);
		}
	}
	int aa = 1;
}

void rayTraceScreen::Trace(rayTraceWorld* world)
{
	double count = 0.0;
	int step = 0;
	int logStep = 5;
	cout << "ATTENTION:CPU rayTracing..." << endl;
	for (int j = 0; j < h; j++)
	{
		for (int i=0;i<w;i++)
		{
			auto& ray = rays[i][j];

			if (!ray.bStopTrace)
			{
				auto info = ray.Trace(world);
				GatherInfo(world, info, i, j);
				if (world->nowBounce < world->bounceNum)
				{
					ray.Bounce(info);
				}
			}

			count += 1.0;
			double percen = count /(w*h) ;
			if (int(percen * 100 / logStep) == step)
			{
				str log = "tracing ";
				log += percen * 100;
				log += "%...";
				cout << log << endl;
				//cout << ray.dir.ToStr() << endl;
				step += 1;
			}
		}
	}
	cout << "ATTENTION:CPU rayTracing done" << endl;
}

void rayTraceScreen::GatherInfo(rayTraceWorld* world, const TraceInfo& info, int i, int j)
{
	//!!!
	debugFrameBuffer[i][j] = rays[i][j].debugColor;
	colorBuffer[i][j] = rays[i][j].color;
	if (info.bHit)
	{
		normalBuffer[i][j] = info.hitN;
		posBuffer[i][j] = info.hitPos;
	}

}