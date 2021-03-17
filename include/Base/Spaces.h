#pragma once 

#include "Taiji.h"

class Space : public Yang
{

};

class Time : public Ying
{
	THISY(Time)
};

class TimeI
{
public:
	double ii=-1.0;
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

//??? 一个完整高效的三维点云集合体
class Space3DI
{
public:
	float ii[3] = {0,1,2};
};

class Space3DR : public R
{
	THISR(Space3D)
};

class MinkowskiSpace : public Time, public Space3D
{
	THISY(MinkowskiSpace)
};

class MinkowskiSpaceR : public R
{
	THISR(MinkowskiSpace)
};