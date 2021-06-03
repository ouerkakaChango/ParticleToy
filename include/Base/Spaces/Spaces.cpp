#include "Spaces.h"

#include "../Utility.h"
#include <iostream>
#include <fstream>
#include <cmath>

using std::endl;
using std::cout;

Time::Time()
{
	auto ti = new TimeI;
	i += ti;
	r += new TimeR(this);
}

void TimeI::Resize(int newLen)
{
	timeStamps.resize(newLen);
}

void TimeI::InitByFrameSec(double frameSec)
{
	for (int inx = 0; inx < timeStamps.size(); inx++)
	{
		timeStamps[inx] = inx * frameSec;
	}
}

void TimeR::SayI()
{
	auto ti = Cast<TimeI*>(y->i[0]);
	std::cout << " timeNum:" << ti->timeStamps.size();
}

//### Space3D
Space3D::Space3D()
{
	auto ti = new Space3DI;
	i += ti;
	auto to = new Space3DO;
	o += to;
	r += new Space3DR(this);
}
//### Space3D

//### Space3DO
void Space3DO::Resize(int newLen)
{
	evolvers.resize(newLen);
}

void Space3DO::InitDefault(int frameNum)
{
	evolvers.resize(frameNum-1);
	evolvers.allset(&defaultEvolver);
}
//### Space3DO

void Space3DI::Resize(int newLen)
{
	spaces.resize(newLen);
}

void Space3DR::SayI()
{
	auto ti = Cast<Space3DI*>(y->i[0]);
	std::cout << " spaceNums:" << ti->spaces.size();
}

void Space3DR::SayO()
{
	auto to = Cast<Space3DO*>(y->o[0]);
	std::cout << " evolverNums:" << to->evolvers.size();
}

//### MinkowskiSpace
MinkowskiSpace::MinkowskiSpace()
{
	//Set I
	for (auto obj : Time::i)
	{
		i += obj;
	}

	for (auto obj : Space3D::i)
	{
		i += obj;
	}

	//Set O
	for (auto obj : Space3D::o)
	{
		o += obj;
	}

	//Set R
	r += new MinkowskiSpaceR(this);
	for (auto obj : Time::r)
	{
		r += obj;
	}

	for (auto obj : Space3D::r)
	{
		r += obj;
	}
}

void MinkowskiSpace::SetFrameSettings(int frameNum_, double frameSec_)
{
	frameNum = frameNum_;
	frameSec = frameSec_;
	for (auto obj : i)
	{
		obj->Resize(frameNum);
	}

	auto spaceo = Cast<Space3DO*>(o[0]);
	spaceo->InitDefault(frameNum);

	auto timeI = Cast<TimeI*>(i[0]);
	timeI->InitByFrameSec(frameSec);
}

void MinkowskiSpace::AddPntNow(const Pnt& pnt)
{
	auto& spaces = Cast<Space3DI*>(i[1])->spaces;
	spaces[F].AddPnt(pnt);
}

void MinkowskiSpace::AddPntNow(str name, P pos, str rule)
{
	auto& spaces = Cast<Space3DI*>(i[1])->spaces;
	spaces[F].AddPnt(name, pos, rule);
}

void MinkowskiSpace::AddTriNow(str name, const Tri& tri, str rule)
{
	auto& spaces = Cast<Space3DI*>(i[1])->spaces;
	spaces[F].AddTri(name, tri, rule);
}

void MinkowskiSpace::EvolveFrame(int prevFrame)
{
	F = prevFrame;
	auto& spaces = Cast<Space3DI*>(i[1])->spaces;
	auto& evolvers = Cast<Space3DO*>(o[0])->evolvers;
	TickInfo tickInfo;
	tickInfo.physic = &evolvers[prevFrame]->physic;
	tickInfo.F = F;
	for (auto& func : beforeTickFuncs)
	{
		func(tickInfo);
	}
	if (prevFrame == 0)
	{
		evolvers[prevFrame]->EvolveBegin(spaces[0], spaces[1], frameSec);
	}
	else
	{
		evolvers[prevFrame]->Evolve(spaces[prevFrame - 1], spaces[prevFrame], spaces[prevFrame + 1], frameSec);
	}
}
//### MinkowskiSpace

void MinkowskiSpaceR::SayI()
{
	for (int inx = 1; inx < y->r.size(); inx++)
	{
		y->r[inx]->SayI();
	}
}

void MinkowskiSpaceR::SayO()
{
	for (int inx = 1; inx < y->r.size(); inx++)
	{
		y->r[inx]->SayO();
	}
}

void MinkowskiSpaceR::PutPnt(const Pnt& pnt)
{
	recordPnts += pnt;
	pntFrames += int2(y->F,y->frameNum-1);
	y->AddPntNow(pnt);
}

void MinkowskiSpaceR::PutPnt(str name, P pos, str rule)
{
	y->AddPntNow(name, pos, rule);
}

void MinkowskiSpaceR::PutTri(str name, const Tri& tri, str rule)
{
	y->AddTriNow(name, tri, rule);
}

void MinkowskiSpaceR::PutTri(str name, const arr<Tri>& tris, str rule)
{
	for (int inx = 0;inx<tris.size();inx++)
	{
		auto& tri = tris[inx];
		y->AddTriNow(name, tri, rule);
	}
}

void MinkowskiSpaceR::Evolve(int begin)
{
	auto& spaces = Cast<Space3DI*>(y->i[1])->spaces;
	int frame = begin;
	while (frame != y->frameNum-1)
	{
		y->EvolveFrame(frame);
		frame += 1;
	}
}

void MinkowskiSpaceR::SetGravity(P gravity)
{
	//由于是设置默认G,而不是在某F设置G，所以只要设置默认Evolver的G就好
	auto& evolver = Cast<Space3DO*>(y->o[0])->defaultEvolver;
	evolver.physic.g = gravity;
}

void MinkowskiSpaceR::PntInsForce(int F, str name, P force)
{
	auto& evolver = Cast<Space3DO*>(y->o[0])->evolvers[F];
	evolver->physic.insForces += force;
}

void MinkowskiSpaceR::MoveToFrame(int F)
{
	y->F = F;
}

void MinkowskiSpaceR::AddBeforeTickFunc(TickFunc tickFunc)
{
	y->beforeTickFuncs += tickFunc;
}

void MinkowskiSpaceR::Say()
{
	auto& times = Cast<TimeI*>(y->i[0])->timeStamps;
	auto& spaces = Cast<Space3DI*>(y->i[1])->spaces;
	std::cout << "3\n";
	std::cout << "frame    t    pos    v\n";
	for (int inx = 0; inx < y->frameNum; inx++)
	{
		std::cout << inx << "    " << times[inx]<<"    "<<spaces[inx].InfoString().data<<"\n";
	}
}

void MinkowskiSpaceR::OutputPntTrajTxt(const str& filePath)
{
	auto& times = Cast<TimeI*>(y->i[0])->timeStamps;
	auto& spaces = Cast<Space3DI*>(y->i[1])->spaces;

	std::cout << "Writing File...\n";
	std::ofstream f(filePath.data, std::ios::out);
	//###
	f << recordPnts.size() <<endl;
	for (int pntInx = 0; pntInx < recordPnts.size(); pntInx++)
	{
		auto& pnt = recordPnts[pntInx];
		auto& pntFrame = pntFrames[pntInx];

		arr<str> traj;
		int startFrame = pntFrame.x, endFrame = pntFrame.y;
		bool bFirstHasLine = true;
		for (int inx = pntFrame.x; inx <= pntFrame.y; inx++)
		{
			str info = spaces[inx].PntInfo(pnt.name, "pos");
			if (info != "")
			{
				if (bFirstHasLine)
				{
					startFrame = inx;
					bFirstHasLine = false;
				}
				traj += info;
			}
			else if(!bFirstHasLine)
			{
				endFrame = inx-1;
				break;
			}
		}
		//输出txt帧号从1开始（Houdini方式）
		f << pnt.TxtHeadString() << " " << (int2(startFrame,endFrame) + int2(1, 1)).ToStr() << endl;
		for (auto& line : traj)
		{
			f << line << endl;
		}
		f << endl;
	}
	//###
	std::cout << "File write done at " << filePath.data << "\n";
}

void MinkowskiSpaceR::DebugSay()
{
	auto& times = Cast<TimeI*>(y->i[0])->timeStamps;
	auto& spaces = Cast<Space3DI*>(y->i[1])->spaces;
	for (int inx = 0; inx < y->frameNum; inx++)
	{
		std::cout << spaces[inx].InfoString("pos v") << "\n";
	}
}

void MinkowskiSpaceR::DebugOutput(const str& filePath)
{
	auto& times = Cast<TimeI*>(y->i[0])->timeStamps;
	auto& spaces = Cast<Space3DI*>(y->i[1])->spaces;

	std::cout << "Writing File...\n";
	std::ofstream f(filePath.data, std::ios::out);
	for (int inx = 0; inx < y->frameNum; inx++)
	{
		f << spaces[inx].InfoString("pos", 5).data << "\n";
	}
	std::cout << "File write done at " << filePath.data << "\n";
}