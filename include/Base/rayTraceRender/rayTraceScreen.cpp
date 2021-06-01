#include "rayTraceScreen.h"

#include "rayTraceWorld.h"
#include <iostream>
using std::cout;
using std::endl;

rayTraceScreen::rayTraceScreen(int w_, int h_):
	w(w_),h(h_)
{
	InitRays();
}

void rayTraceScreen::InitRays()
{
	//屏幕x属于[-1,1]，y根据分辨率变化的screen，eye在(0,1,0)对每个像素中心发射ray(trace束)
	double dx = 2.0 / w;
	double ylen = (2.0*h) / w;
	
	P startPos = P(-1.0, -ylen, 0.0) + 0.5*P(dx, dx, 0.0);
	for (int j = 0; j < h; j++)
	{
		for (int i = 0; i < w; i++)
		{
			P endPos = startPos + i*P(dx,dx,0.0);
			rays += TraceRay(P(0,0,1),endPos);
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
	for (auto& ray : rays)
	{
		ray.Trace(world);
		count += 1.0;
		double percen = count / rays.size();
		if (int(percen * 100 / logStep) == step)
		{
			str log = "tracing ";
			log += percen*100;
			log += "%...";
			cout << log << endl;
			step += 1;
		}
	}
	cout << "ATTENTION:CPU rayTracing done" << endl;
}