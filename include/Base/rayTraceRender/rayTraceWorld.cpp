#include "rayTraceWorld.h"

#include "Render/Material.h"
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
			nowBounce += 1;
			screen->Trace(this);
		}
	}
}

TraceInfo rayTraceWorld::SDF(P pos)
{
	//!!!
	TraceInfo re;
	re.dis = maxSDF;
	if (objs.size() == 0)
	{
		return re;
	}
	double minDis = maxSDF;
	Object* tObj = nullptr;
	for (auto& obj : objs)
	{
		if (obj->shape != nullptr)
		{
			double tDis = obj->shape->SDF(pos);
			if (tDis < minDis)
			{
				minDis = tDis;
				tObj = obj;
			}
		}
	}
	re.dis = minDis;
	re.obj = tObj;
	return re;
}

P rayTraceWorld::CalculateMaterial(const TraceInfo& info)
{
	P color;
	if (info.obj && info.obj->shape)
	{
		auto mat = info.obj->material;
		if (mat != nullptr)
		{
			P n = info.hitN;
			P v = -info.dir;
			color = mat->Calculate(lights, n, v);
		}
	}
	return color;
}

P rayTraceWorld::BlendColor(const TraceInfo& info)
{
	P color = info.color;
	if (info.obj && info.obj->shape)
	{
		auto mat = info.obj->material;
		if (mat != nullptr)
		{
			if (typeStr(mat->i[0]) == "BlinnPhongI")
			{
				auto param = Cast<BlinnPhongI*>(mat->i[0]);
				return color + param->reflectness * info.color;
			}
		}
	}
	return color;
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

void rayTraceWorldR::PutLight(DirectionalLight* light)
{
	y->lights += light;
}

void rayTraceWorldR::Evolve()
{
	y->Evolve();
}

void rayTraceWorldR::SaveScreenBufferFrame(rayTraceScreen* screen, const str& bufferName, const str& filePath)
{
	std::cout << "ATTENTION: CPU Writing FrameBuffer...\n";
	std::ofstream f(filePath.data, std::ios::out);
	f << screen->w << " " << screen->h << endl;
	for (int j = 0; j < screen->h; j++)
	{
		for (int i = 0; i < screen->w; i++)
		{
			P pixel;
			if (bufferName == "debug")
			{
				pixel = screen->debugFrameBuffer[i][j];
			}
			else if (bufferName == "color")
			{
				pixel = screen->colorBuffer[i][j];
			}
			else if (bufferName == "normal")
			{
				pixel = screen->normalBuffer[i][j];
			}
			else if (bufferName == "pos")
			{
				pixel = screen->posBuffer[i][j];
			}
			int R = Cast<int>(pixel.x * 255);
			int G = Cast<int>(pixel.y * 255);
			int B = Cast<int>(pixel.z * 255);
			f << R << " " << G << " " << B << endl;
		}
	}
	std::cout << "ATTENTION: CPU Writing FrameBuffer done\n";
}
//### rayTraceWorldR