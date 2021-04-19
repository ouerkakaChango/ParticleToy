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
	void AddPnt(const Pnt& pnt);
	void AddTri(str name, const Tri& tri, str rule);
	void CopyFrom(const Fast3D& prev);
	str InfoString(int precision=5);
	str InfoString(str filter, int precision=5);
	str PntInfo(const str& name, const str& attrib);


	arr<Pnt> pnts;
	arr<Tri> triArr;
};