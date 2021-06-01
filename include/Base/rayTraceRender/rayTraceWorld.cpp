#include "rayTraceWorld.h"

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

void rayTraceWorld::SetTraceSettings(int bounceNum_)
{
	bounceNum = bounceNum_;
}

void rayTraceWorld::Evolve()
{
	for (int i = 0; i < bounceNum; i++)
	{
		for (auto& screen : screens)
		{
			screen->Trace(this);
		}
	}
}

double rayTraceWorld::SDF(P pos)
{
	//!!!
	if (objs.size() == 0)
	{
		return maxSDF;
	}
	double minDis = maxSDF;
	for (auto& obj : objs)
	{
		if (obj->shape != nullptr)
		{
			double tDis = obj->shape->SDF(pos);
			if (tDis < minDis)
			{
				minDis = tDis;
			}
		}
	}
	return minDis;
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

void rayTraceWorldR::PutShape(Shape* shape, const str& name)
{
	auto obj = new Object;
	obj->SetShape(shape);
	obj->name = name;

	y->objs += obj;
}

void rayTraceWorldR::PutScreen(rayTraceScreen* screen)
{
	y->screens += screen;
}

void rayTraceWorldR::Evolve()
{
	y->Evolve();
}

void rayTraceWorldR::SaveScreenDebugFrame(rayTraceScreen* screen, const str& filePath)
{
	std::cout << "ATTENTION: CPU Writing FrameBuffer...\n";
	std::ofstream f(filePath.data, std::ios::out);
	f << screen->w << " " << screen->h << endl;
	for (int j = 0; j < screen->h; j++)
	{
		for (int i = 0; i < screen->w; i++)
		{
			auto pixel = screen->debugFrameBuffer[i][j];
			int R = pixel.x * 255;
			int G = pixel.y * 255;
			int B = pixel.z * 255;
			f << R << " " << G << " " << B << endl;
		}
	}
	std::cout << "ATTENTION: CPU Writing FrameBuffer done\n";
}
//### rayTraceWorldR