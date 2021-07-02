#include "RTTraceRequest.h"

#include <fstream>
#include <iostream>
#include <direct.h>
#include "../TraceRay.h"
#include "../rayTraceWorld.h"
#include "../rayTraceScreen.h"
#include "../TraceMode/SDFSphereTrace/TraceRayI_SDFSphereMonteCarlo.h"

using std::endl;
using std::cout;
using std::wcout;

//### RTTraceRequest
RTTraceRequest::RTTraceRequest()
{
	
}

void RTTraceRequest::InitData(rayTraceWorld* world_, rayTraceScreen* screen, const str& outPath, RequestMode mode_)
{
	world = world_;
	mode = mode_;
	if (world->traceMode == rayTraceMode_SDFSphere)
	{
		auto to = new RTTraceRequestO_SDFSphere(this);
		to->InitData(world,screen);
		o += to;
	}
	if (mode == RequestMode_txt)
	{
		auto ti = new RTTraceRequestI_txt;
		ti->outPath = outPath;
		ti->results.resize(1);
		ti->o = Cast<RTTraceRequestO*>(o[0]);
		i += ti;
	}
}

void RTTraceRequest::SetRequest(rayTraceScreen* screen_)
{
	screen = screen_;
}

void RTTraceRequest::PrepareForNext()
{
	auto data = Cast<RTTraceRequestO*>(o[0]);
	data->PrepareForNext();
}

void RTTraceRequest::SendAndWaitGetResult()
{
	//???
	auto ti = Cast<RTTraceRequestI*>(i[0]);

	auto& ray = screen->rays[0][0];
	auto rayi = Cast<TraceRayI*>(ray.i[0]);

	if (typeStr(*rayi) == "class TraceRayI_SDFSphereMonteCarlo")
	{
		int nowReqNum = 1;
		if (world->nowBounce > 1)
		{
			nowReqNum = TraceRayI_SDFSphereMonteCarlo::spp;
		}
		for (int inx = 0; inx < nowReqNum;inx++ )
		{
			ti->SendSingleRequest(sendInx);
			sendInx += 1;
			//???
			ti->CallCalculation();
			ti->WaitForResult(sendInx);
		}
	}
}

TraceInfo RTTraceRequest::GetResultAndSet(int x, int y)
{
	TraceInfo re;

	auto ti = Cast<RTTraceRequestI*>(i[0]);

	auto& ray = screen->rays[x][y];

	auto rayi = Cast<TraceRayI*>(ray.i[0]);

	if (typeStr(*rayi) == "class TraceRayI_SDFSphereMonteCarlo")
	{
		if (world->nowBounce == 1)
		{
			ti->GetResultAndSet(re, x, y, ray);
		}
	}

	return re;
}
//### RTTraceRequest

//### RTTraceRequestO
RTTraceRequestO::RTTraceRequestO(RTTraceRequest* y_):y(y_)
{

}
//### RTTraceRequestO

//### RTTraceRequestO_SDFSphere
RTTraceRequestO_SDFSphere::RTTraceRequestO_SDFSphere(RTTraceRequest* y_):
	RTTraceRequestO(y_)
{

}
void RTTraceRequestO_SDFSphere::InitData(rayTraceWorld* world_, rayTraceScreen* screen)
{
	world = world_;
	w = screen->w;
	h = screen->h;

	auto& ray = screen->rays[0][0];
	auto rayi = Cast<TraceRayI*>(ray.i[0]);

	if (typeStr(*rayi) == "class TraceRayI_SDFSphereMonteCarlo")
	{
		y->requestNum = 1 + TraceRayI_SDFSphereMonteCarlo::spp * (world->bounceNum-1);
	}
	else
	{
		abort();
	}
}

void RTTraceRequestO_SDFSphere::PrepareForNext()
{

}

//### RTTraceRequestO_SDFSphere

//### RTTraceRequestI_txt
void RTTraceRequestI_txt::SendRequest()
{
	if (typeStr(*o) == "class RTTraceRequestO_SDFSphere")
	{
		abort();
	}
	else
	{
		abort();
	}
}

void RTTraceRequestI_txt::SendSingleRequest(int reqInx)
{
	if (typeStr(*o) == "class RTTraceRequestO_SDFSphere")
	{
		auto to = Cast<RTTraceRequestO_SDFSphere*>(o);
		str tFilePath = outPath + "\\traceReq" + str(reqInx)+".txt";
		std::ofstream f(tFilePath.data, std::ios::out);
		f << to->w << " " << to->h << " " << endl;
		if (reqInx == 0)
		{
			cout << "Writing req0 ..." << endl;
			for (int j = 0; j < to->h; j++)
			{
				for (int i = 0; i < to->w; i++)
				{
					auto& ray = o->y->screen->rays[i][j];
					f << ray.ori.ToStr() << " " << ray.dir.ToStr() << endl;
				}
			}
		}
		else
		{
			int subRayInx = (reqInx-1) % TraceRayI_SDFSphereMonteCarlo::spp;
			cout << "Writing req" << reqInx << " subRay" << subRayInx << "..." << endl;
			for (int j = 0; j < to->h; j++)
			{
				for (int i = 0; i < to->w; i++)
				{
					auto& ray = o->y->screen->rays[i][j];
					auto subRay = Cast<TraceRayI_SDFSphereMonteCarlo*>(ray.i[0])->subRays[subRayInx];
					f << subRay.ori.ToStr() << " " << subRay.dir.ToStr() << endl;
				}
			}
		}
		f.close();
	}
	else
	{
		abort();
	}
}

void RTTraceRequestI_txt::CallCalculation()
{
	//???
	//将objs的信息元编程到.py代码，并运行calculation

	//wchar_t   buffer[MAX_PATH];
	//_wgetcwd(buffer, MAX_PATH);
	//std::wcout.imbue(std::locale("chs"));
	//wcout << buffer;
	//!!! 现在默认路径必须英文，以后再改wchar做的Path类
	char cwd[MAX_PATH];
	_getcwd(cwd, MAX_PATH);
	_chdir(rayTraceGod.pythonWorkPath.c_str());
	system("RUN_testReq.bat");
	_chdir(cwd);
}

void RTTraceRequestI_txt::WaitForResult(int reqInx)
{
	//wait check result file is exist

	//read into results
	//???
	for (auto& page : results)
	{
		str tFilePath = rayTraceGod.optimizeWorkPath + "\\TraceResult";
		tFilePath += "\\traceRes0.txt";
		std::ifstream f(tFilePath.data);
		int w, h;
		double tDis=-1.0, tObj=-1;
		f >> w; f >> h;
		page.resize(w, h);
		for (int j = 0; j < h; j++)
		{
			for (int i = 0; i < w; i++)
			{
				f >> page[i][j].traceDis >> page[i][j].traceObj;
			}
		}
	}
}

void RTTraceRequestI_txt::GetResultAndSet(TraceInfo& info, int x, int y, const TraceRay& ray)
{
	auto world = o->y->world;
	if (world->nowBounce == 1)
	{
		auto& page = results[0];
		info.dis = page[x][y].traceDis;
		if (info.dis > 0)
		{
			info.bHit = true;
			info.dir = ray.dir;
			info.hitPos = ray.ori + info.dis * info.dir;
			info.obj = world->objs[page[x][y].traceObj];
		}
	}
}
//### RTTraceRequestI_txt