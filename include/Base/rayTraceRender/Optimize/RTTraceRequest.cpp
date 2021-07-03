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
	auto ti = Cast<RTTraceRequestI*>(i[0]);
	ti->PrepareForNext();
}

void RTTraceRequest::SendAndWaitGetResult()
{
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
			ti->CallCalculation(sendInx);
			ti->WaitForResult(sendInx);
			sendInx += 1;
		}
	}
}

void RTTraceRequest::GetResult(arr<TraceInfo>& infos, int x, int y)
{
	auto ti = Cast<RTTraceRequestI*>(i[0]);
	ti->GetResult(infos, x, y);
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
		//!!!
		auto& ray = o->y->screen->rays[0][0];
		auto rayi = Cast<TraceRayI*>(ray.i[0]);
		if (typeStr(*rayi) == "class TraceRayI_SDFSphereMonteCarlo")
		{
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
				int subRayInx = (reqInx - 1) % TraceRayI_SDFSphereMonteCarlo::spp;
				cout << "Writing req" << reqInx << " subRay" << subRayInx << "..." << endl;
				for (int j = 0; j < to->h; j++)
				{
					for (int i = 0; i < to->w; i++)
					{
						auto& ray = o->y->screen->rays[i][j];
						if (!ray.bStopTrace)
						{
							auto subRay = Cast<TraceRayI_SDFSphereMonteCarlo*>(ray.i[0])->subRays[subRayInx];
							f << subRay.ori.ToStr() << " " << subRay.dir.ToStr() << endl;
						}
						else
						{
							f << endl;
						}
					}
				}
			}
		}
		else
		{
			abort();
		}
		f.close();
	}
	else
	{
		abort();
	}
}

void RTTraceRequestI_txt::CallCalculation(int reqInx)
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
	//system("RUN_testReq.bat");
	str cmdStr = "python testOutResult.py ";
	cmdStr += "traceReq" + str(reqInx) + ".txt ";
	cmdStr += "TraceResult/traceRes" + str(reqInx) + ".txt";
	system(cmdStr.c_str());
	_chdir(cwd);
}

void RTTraceRequestI_txt::WaitForResult(int reqInx)
{
	//wait check result file is exist
	//read into results
	auto& page = results[reqInx];
	str tFilePath = rayTraceGod.optimizeWorkPath + "\\TraceResult";
	tFilePath += "\\traceRes"+str(reqInx)+".txt";
	std::ifstream f(tFilePath.data);
	while (!f.is_open())
	{
		f.open(tFilePath.data);
	}
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

void RTTraceRequestI_txt::GetResult(arr<TraceInfo>& infos, int x, int y)
{
	auto world = o->y->world;
	auto& mainRay = o->y->screen->rays[x][y];
	auto rayi = Cast<TraceRayI*>(mainRay.i[0]);

	auto loadInfo = [&](TraceRay& ray, TraceInfo& info,const arr2<RTTraceResult>& page)
	{
		info.dis = page[x][y].traceDis;
		if (info.dis > 0)
		{
			info.bHit = true;
			info.dir = ray.dir;
			info.hitPos = ray.ori + info.dis * info.dir;
			info.obj = world->objs[page[x][y].traceObj];
		}
	};

	if (typeStr(*rayi) == "class TraceRayI_SDFSphere")
	{
		infos.resize(1);
		loadInfo(mainRay,infos[0],results[0]);
	}
	else if (typeStr(*rayi) == "class TraceRayI_SDFSphereMonteCarlo")
	{
		if (world->nowBounce == 1)
		{
			infos.resize(1);
			loadInfo(mainRay,infos[0], results[0]);
		}
		else
		{
			infos.resize(TraceRayI_SDFSphereMonteCarlo::spp);
			
			for (int inx = 0; inx < TraceRayI_SDFSphereMonteCarlo::spp; inx++)
			{
				auto& ray = Cast<TraceRayI_SDFSphereMonteCarlo*>(rayi)->subRays[inx];
				loadInfo(ray, infos[inx], results[lastStamp+inx]);
			}
		}
	}
}

void RTTraceRequestI_txt::PrepareForNext()
{
	auto world = o->y->world;
	auto& ray = o->y->screen->rays[0][0];
	auto rayi = Cast<TraceRayI*>(ray.i[0]);
	if (typeStr(*rayi) == "class TraceRayI_SDFSphereMonteCarlo")
	{
		results.addEmptyElems(TraceRayI_SDFSphereMonteCarlo::spp);
		if (world->nowBounce == 1)
		{
			lastStamp = 1;
		}
		else
		{
			lastStamp += TraceRayI_SDFSphereMonteCarlo::spp;
		}
	}
	
}
//### RTTraceRequestI_txt