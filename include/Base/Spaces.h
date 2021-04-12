#pragma once 

#include "Taiji.h"
#include "Fast3D.h"
#include "Evolver.h"
#include "FastGrid.h"

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
	void AddPntNow(str name, P pos, str rule);
	void AddTriNow(str name, Tri tri, str rule);
	void EvolveFrame(int prevFrame);

	double frameSec;
	int frameNum;
	int F = 0;
};

class MinkowskiSpaceR : public R
{
	THISR(MinkowskiSpace)

	void PutPnt(str name, P pnt, str rule);
	void PutTri(str name, Tri tri, str rule);
	void Evolve(int begin);
	void SetGravity(P gravity);
	void PntInsForce(int F, str name, P force);

	void SayO() override;
	void Say();
	void DebugSay();
};

//### Grid
class Grid : public EuclideanSpace
{
	THISY(Grid)
	template<class DataClass>
	void SetGridSettings(int edgeNum, double cellLength)
	{
		auto ti = new GridI<DataClass>;
		i += ti;
		auto& grid = ti->grid;
		grid.SetSize(edgeNum, cellLength);
	}
};

template <class DataClass>
class GridI : public ClassI
{
public:
	FastGrid<DataClass> grid;
};

class GridO : public ClassO
{
public:

};

class GridR : public R
{
	THISR(Grid)
	void DiamondTerrain(int detailLevel);
};
//### Grid3D