#pragma once 

#include "Taiji.h"
#include "Fast3D.h"

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

class Space3DO : public Space3D
{

};
typedef Space3DO Coordinate3D;

class Space3DI : public ClassI
{
public:
	void Resize(int newLen) override;
	arr<Fast3D> spaces;
};

class Space3DR : public R
{
	THISR(Space3D)
};

class MinkowskiSpace : public Time, public Space3D
{
	THISY(MinkowskiSpace)
	void SetFrameSettings(int frameNum_, double frameSec_);
	void AddPntNow(str name, P pos);

	double frameSec;
	int frameNum;
	int F = 0;
};

class MinkowskiSpaceR : public R
{
	THISR(MinkowskiSpace)
	void PutPnt(str name, P pnt, str rule);
	void Evolve(int begin);
};