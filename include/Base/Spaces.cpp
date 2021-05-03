#include "Spaces.h"

#include "Utility.h"
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
	//??? debug
	if (prevFrame == 464)
	{
		int aa = 1;
	}
	if (prevFrame == 254)
	{
		int aa = 1;
	}
	//___
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
//### Grid
Grid::Grid()
{
	//Set R
	r += new GridR(this);
}
//### Grid

//### EasyTerrainAlgo
void EasyTerrainAlgo::Create(GridI<double>* terrain, int detailLevel_, double initH_, double roughness_)
{
	if (detailLevel_ <= 0 || roughness_<0)
	{
		abort();
		return;
	}
	if (terrain == nullptr)
	{
		abort();
		return;
	}
	detailLevel = detailLevel_;
	initH = initH_;
	roughness = roughness_;
	grid = &terrain->grid;
	
	InitCorner(initH_/2.0);

	bool bLoop = true;
	int detailInx = 0;
	while (bLoop)
	{
		stepSize = (grid->datas.x - 1) / std::pow(2, detailInx);
		Square();
		if (bMaxFrac)
		{
			break;
		}
		Diamond();
		if (bMaxFrac)
		{
			break;
		}
		detailInx += 1;
		if (detailInx == detailLevel)
		{
			break;
		}
	}
}

void EasyTerrainAlgo::InitCorner(double h)
{
	int stepSize = grid->datas.x - 1;
	P2 inx1 = P2(0, 0);
	P2 inx2 = P2(0, 0) + P2(1, 0)*stepSize;
	P2 inx3 = P2(0, 0) + P2(1, 1)*stepSize;
	P2 inx4 = P2(0, 0) + P2(0, 1)*stepSize;

	grid->Set(inx1, h);
	grid->Set(inx2, h);
	grid->Set(inx3, h);
	grid->Set(inx4, h);
}

double EasyTerrainAlgo::Offset()
{
	return roughness * stepSize * (2 * rand01() - 1)* initH;
}

double EasyTerrainAlgo::H(double h1, double h2, double h3, double h4)
{
	return (h1 + h2 + h3 + h4) / 4 + Offset();
}

void EasyTerrainAlgo::Square()
{
	if (stepSize < 2)
	{
		bMaxFrac = true;
		return;
	}
	P2 inxStart(0, 0);
	int edgeNum = Cast<int>((grid->datas.x - 1) / stepSize);
	for (int j = 0; j < edgeNum; j++)
	{
		for (int i = 0; i < edgeNum; i++)
		{
			P2 inx1 = inxStart;
			P2 inx2 = inxStart + P2(1, 0)*stepSize;
			P2 inx3 = inxStart + P2(1, 1)*stepSize;
			P2 inx4 = inxStart + P2(0, 1)*stepSize;
			P2 inxCenter = inxStart + P2(0.5, 0.5)*stepSize;

			double h1 = grid->Get(inx1);
			double h2 = grid->Get(inx2);
			double h3 = grid->Get(inx3);
			double h4 = grid->Get(inx4);
			double h = H(h1, h2, h3, h4);
			grid->Set(inxCenter, h);
			inxStart += P2(1, 0)*stepSize;
		}
		inxStart.x = 0;
		inxStart.y += stepSize;
	}
}

void EasyTerrainAlgo::Diamond()
{
	if (stepSize < 2)
	{
		bMaxFrac = true;
		return;
	}
	P2 inxStart(0, 0);
	int edgeNum = Cast<int>((grid->datas.x - 1) / stepSize);
	for (int j = 0; j < edgeNum; j++)
	{
		for (int i = 0; i < edgeNum; i++)
		{
			P2 inx1 = inxStart;
			P2 inx2 = inxStart + P2(1, 0)*stepSize;
			P2 inx3 = inxStart + P2(1, 1)*stepSize;
			P2 inx4 = inxStart + P2(0, 1)*stepSize;
			P2 inxCenter = inxStart + P2(0.5, 0.5)*stepSize;

			double h1 = grid->Get(inx1);
			double h2 = grid->Get(inx2);
			double h3 = grid->Get(inx3);
			double h4 = grid->Get(inx4);
			double hc = grid->Get(inxCenter);

			//dia1:1-2-c
			SubDiamond(inx1,inx2,inxCenter,h1,h2,hc);

			//dia2:2-3-c
			SubDiamond(inx2, inx3, inxCenter, h2, h3, hc);

			//dia3:3-4-c
			SubDiamond(inx3, inx4, inxCenter, h3, h4, hc);

			//dia4:4-1-c
			SubDiamond(inx4, inx1, inxCenter, h4, h1, hc);
			
			inxStart += P2(1, 0)*stepSize;
		}
		inxStart.x = 0;
		inxStart.y += stepSize;
	}
}

void EasyTerrainAlgo::SubDiamond(const P2& inx1, const P2& inx2, const P2& inxCenter,
	double h1, double h2, double hc)
{
	P2 inxC1 = (inx1 + inx2) / 2;
	P2 inxO1 = inxC1 * 2 - inxCenter;
	if (inxO1 >= P2(0, 0) && inxO1 < P2(grid->datas.x,grid->datas.y))
	{
		double ho = grid->Get(inxO1);
		grid->Set(inxC1, H(h1, h2, hc, ho));
	}
	else
	{
		grid->Set(inxC1, H(h1, h2, hc));
	}
}
//### EasyTerrainAlgo

//### GridR
void GridR::SayI()
{
	auto& grid = Cast<GridI<double>*>(y->i[0])->grid;
	auto& pnts = grid.pnts;
	for (int j = 0; j < pnts.y; j++)
	{
		for (int i = 0; i < pnts.x; i++)
		{
			auto& p = pnts[i][j];
			std::cout <<"("<< p.ToStr().data<<"):"<< grid.datas[i][j]<<" ";
		}
		std::cout << "\n";
	}
}

void GridR::EasyTerrain(double initH, double roughness, int detailLevel)
{
	if (detailLevel == 0)
	{
		abort();
		return;
	}
	auto to = new EasyTerrainAlgo;
	y->o += to;

	auto grid = Cast<GridI<double>*>(y->i[0]);
	to->Create(grid, detailLevel, initH, roughness);
}

void GridR::TerrainToTri(arr<Tri>& triArr)
{
	auto& fGrid = Cast<GridI<double>*>(y->i[0])->grid;
	FastGridToTri(fGrid, triArr);
}

void GridR::DebugSay(int mode)
{
	auto& grid = Cast<GridI<double>*>(y->i[0])->grid;
	for (int j = 0; j < grid.datas.y; j++)
	{
		for (int i = 0; i < grid.datas.x; i++)
		{
			P p(grid.pnts[i][j], "zx");
			p.y = grid.datas[i][j];
			if (mode == 1)
			{
				std::cout << p.ToStr().data << "\n";
			}
			else if (mode == 2)
			{
				std::cout << p.ToStr().data << "--- "<<P2(i,j).ToStr().data<< "\n";
			}
		}
	}
}

void GridR::DebugOutput(const str& filePath)
{
	std::cout << "Writing File...\n";
	std::ofstream f(filePath.data,std::ios::out);
	auto& grid = Cast<GridI<double>*>(y->i[0])->grid;
	for (int j = 0; j < grid.datas.y; j++)
	{
		for (int i = 0; i < grid.datas.x; i++)
		{
			P p(grid.pnts[i][j], "zx");
			p.y = grid.datas[i][j];
			f << p.ToStr().data << std::endl;
		}
	}
	std::cout << "File write done at "<<filePath.data<<"\n";
}
//### GridR