#pragma once

#include "Container.h"
#include "FastMath.h"
#include "FastGeo.h"
#include "FastPnt.h"

//??? 一个完整高效的三维空间解决方案，看齐VDB,PCL那种
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