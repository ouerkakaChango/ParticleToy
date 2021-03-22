#pragma once

#include "Container.h"

class P
{
public:
	P();
	P(double x_, double y_, double z_);
	double x, y, z;
};

class Pnt
{
public:
	Pnt();
	Pnt(P pos_);
	P pos;
};

//??? 一个完整高效的三维空间解决方案，看齐VDB,PCL那种
class Fast3D
{
public:
	void AddPnt(str name, P pos);

	arr<Pnt> pnts;
};