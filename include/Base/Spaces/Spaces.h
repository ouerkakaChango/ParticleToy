#pragma once 

#include "../Taiji.h"
#include "../Fast3D.h"
#include "../Evolver.h"
#include "../FastGrid.h"
#include "../TickUtility.h"

class Space : public Yang
{

};

class Time : public Ying
{
	THISY(Time)
};

class TimeI : public ClassI
{
public:
	void Resize(int newLen) override;
	void InitByFrameSec(double frameSec);
	arr<double> timeStamps;
};

class TimeR :public R
{
	THISR(Time)
};

class MetricSpace : public Space
{

};

class EuclideanSpace : public MetricSpace
{

};

class Space3D : public EuclideanSpace
{
	THISY(Space3D)
};

class Space3DO : public ClassO
{
public:
	void Resize(int newLen) override;
	void InitDefault(int frameNum);
	arr<Evolver*> evolvers;
	Evolver defaultEvolver;
};

class Space3DI : public ClassI
{
public:
	void Resize(int newLen) override;
	arr<Fast3D> spaces;
};

class Space3DR : public R
{
	THISR(Space3D)
	void SayO() override;
};

//i:time,space3D
//o:spaceD
class MinkowskiSpace : public Time, public Space3D
{
	THISY(MinkowskiSpace)
	void SetFrameSettings(int frameNum_, double frameSec_);
	void AddPntNow(const Pnt& pnt);
	void AddPntNow(str name, P pos, str rule);
	void AddTriNow(str name, const Tri& tri, str rule);
	void EvolveFrame(int prevFrame);

	double frameSec;
	int frameNum;
	int F = 0;
	arr<TickFunc> beforeTickFuncs;
};

class MinkowskiSpaceR : public R
{
	THISR(MinkowskiSpace)

	void PutPnt(const Pnt& pnt);
	void PutPnt(str name, P pos, str rule);
	void PutTri(str name, const Tri& tri, str rule);
	void PutTri(str name, const arr<Tri>& tris, str rule);
	void Evolve(int begin);
	void SetGravity(P gravity);
	void PntInsForce(int F, str name, P force);
	void MoveToFrame(int F);
	void AddBeforeTickFunc(TickFunc tickFunc);

	void SayO() override;
	void Say();
	void OutputPntTrajTxt(const str& filePath);
	void DebugSay();
	void DebugOutput(const str& filePath);

	//记录所有加入的点的初态
	arr<Pnt> recordPnts;
	arr<int2> pntFrames;
};