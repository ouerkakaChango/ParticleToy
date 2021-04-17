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
	void AddPntNow(const Pnt& pnt);
	void AddPntNow(str name, P pos, str rule);
	void AddTriNow(str name, const Tri& tri, str rule);
	void EvolveFrame(int prevFrame);

	double frameSec;
	int frameNum;
	int F = 0;
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

	void SayO() override;
	void Say();
	void DebugSay();
	void DebugOutput(const str& filePath);
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

class TerrainAlgo : public GridO
{
public:
};

class EasyTerrainAlgo : public TerrainAlgo
{
public:
	void Create(GridI<double>* terrain, int detailLevel_, double initH_, double roughness_);
	void InitCorner(double h);
	void Square();
	void Diamond();
	void SubDiamond(const P2& inx1,const P2& inx2, const P2& inxCenter,
		double h1,double h2,double hc);
	double Offset();
	double H(double h1, double h2, double h3, double h4=0.0);

	FastGrid<double>* grid;
	int detailLevel;
	double initH;
	bool bMaxFrac=false;
	double stepSize = -10000.0;
	double roughness = 0.3;
};

class GridR : public R
{
	THISR(Grid)
	void EasyTerrain(double initH, double roughness, int detailLevel = 100);
	void TerrainToTri(arr<Tri>& triArr);
	void DebugSay(int mode=1);
	void DebugOutput(const str& filePath);
};
//### Grid3D