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
//### rayTraceWorld

//### rayTraceWorldR
void rayTraceWorldR::SayI()
{
	for (auto& obj : objs)
	{
		cout << obj->name << endl;
	}
}

void rayTraceWorldR::PutShape(Shape* shape, const str& name)
{
	auto obj = new Object;
	obj->SetShape(shape);
	obj->name = name;

	objs += obj;
}
//### rayTraceWorldR