#include "rayTraceScreen.h"

#include "rayTraceWorld.h"

rayTraceScreen::rayTraceScreen(int w_, int h_):
	w(w_),h(h_)
{
	InitRays();
}

void rayTraceScreen::InitRays()
{
	///???
}

void rayTraceScreen::Trace(rayTraceWorld* world)
{
	for (auto& obj : world->objs)
	{
		for (auto& ray : rays)
		{
			ray.Trace(obj);
		}
	}
}