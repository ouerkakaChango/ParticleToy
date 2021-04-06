#include "Spaces.h"
#include "Utility.h"

#include <iostream>

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

void MinkowskiSpace::AddPntNow(str name, P pos, str rule)
{
	auto& spaces = Cast<Space3DI*>(i[1])->spaces;
	spaces[F].AddPnt(name, pos, rule);
}

void MinkowskiSpace::AddTriNow(str name, Tri tri, str rule)
{
	auto& spaces = Cast<Space3DI*>(i[1])->spaces;
	spaces[F].AddTri(name, tri, rule);
}

void MinkowskiSpace::EvolveFrame(int prevFrame)
{
	auto& spaces = Cast<Space3DI*>(i[1])->spaces;
	auto& evolvers = Cast<Space3DO*>(o[0])->evolvers;
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

void MinkowskiSpaceR::PutPnt(str name, P pos, str rule)
{
	y->AddPntNow(name, pos, rule);
}

void MinkowskiSpaceR::PutTri(str name, Tri tri, str rule)
{
	y->AddTriNow(name, tri, rule);
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

void MinkowskiSpaceR::Say()
{
	auto& times = Cast<TimeI*>(y->i[0])->timeStamps;
	auto& spaces = Cast<Space3DI*>(y->i[1])->spaces;
	std::cout << "3\n";
	std::cout << "frame    t    pos\n";
	for (int inx = 0; inx < y->frameNum; inx++)
	{
		std::cout << inx << "    " << times[inx]<<"    "<<spaces[inx].InfoString().data<<"\n";
	}
}

void MinkowskiSpaceR::DebugSay()
{
	auto& times = Cast<TimeI*>(y->i[0])->timeStamps;
	auto& spaces = Cast<Space3DI*>(y->i[1])->spaces;
	for (int inx = 0; inx < y->frameNum; inx++)
	{
		std::cout << spaces[inx].InfoString(5).data << "\n";
	}
}