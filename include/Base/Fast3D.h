#pragma once

#include "Container.h"
#include "FastMath.h"
#include "FastGeo.h"
#include "FastPnt.h"

//??? һ��������Ч����ά�ռ�������������VDB,PCL����
class Fast3D
{
public:
	void AddPnt(str name, P pos, str rule);
	void AddTri(str name, Tri tri, str rule);
	str InfoString(int precision=3);
	str InfoString(str filter, int precision=3);
	void CopyFrom(const Fast3D& prev);

	arr<Pnt> pnts;
	arr<Tri> triArr;
};