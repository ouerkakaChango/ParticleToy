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
	r += new Space3DR(this);
}
//### Space3D

void Space3DI::Resize(int newLen)
{
	spaces.resize(newLen);
}

void Space3DR::SayI()
{
	auto ti = Cast<Space3DI*>(y->i[0]);
	std::cout << " spaceNums:" << ti->spaces.size();
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
	auto timeI = Cast<TimeI*>(i[0]);
	timeI->InitByFrameSec(frameSec);
}

void MinkowskiSpace::AddPntNow(str name, P pos)
{
	auto& spaces = Cast<Space3DI*>(i[1])->spaces;
	spaces[F].AddPnt(name, pos);
}
//### MinkowskiSpace

void MinkowskiSpaceR::SayI()
{
	for (int inx = 1; inx < y->r.size(); inx++)
	{
		y->r[inx]->SayI();
	}
}

void MinkowskiSpaceR::PutPnt(str name, P pos, str rule)
{
	y->AddPntNow(name, pos);
}

void MinkowskiSpaceR::Evolve(int begin)
{
	auto& spaces = Cast<Space3DI*>(y->i[1])->spaces;
	auto& nowWorld = spaces[begin];
}