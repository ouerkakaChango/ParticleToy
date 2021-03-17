#pragma once 

#include "Taiji.h"

class Space : public Yang
{

};

class Time : public Ying
{
	THIS(Time)
};

class TimeI
{
public:
	double ii=-1.0;
};

class TimeR :public R
{
public:
	TimeR(TimeI* i_);
	void SayI() override;
	TimeI* i;
};

class MetricSpace : public Space
{

};

class EuclideanSpace : public MetricSpace
{

};

class Space3D : public EuclideanSpace
{
	THIS(Space3D)
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
public:
	Space3DR(Space3DI* i_);
	void SayI() override;
	Space3DI* i;
};

class MinkowskiSpace : public Time, public Space3D
{
	THIS(MinkowskiSpace)
};

class MinkowsikiSpaceI : public MinkowskiSpace
{

};