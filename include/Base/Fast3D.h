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

//??? һ��������Ч����ά�ռ�������������VDB,PCL����
class Fast3D
{
public:
	void AddPnt(str name, P pos);

	arr<Pnt> pnts;
};