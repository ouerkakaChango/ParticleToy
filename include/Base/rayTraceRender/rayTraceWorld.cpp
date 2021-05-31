#include "rayTraceWorld.h"

#include <iostream>
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
//### rayTraceWorldR