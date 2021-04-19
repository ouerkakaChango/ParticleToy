#include "Fast3D.h"

#include "EffectSpace.h"

//### Fast3D
void Fast3D::AddPnt(str name, P pos, str rule)
{
	pnts += Pnt(pos, name, rule);
}

void Fast3D::AddPnt(const Pnt& pnt)
{
	pnts += pnt;
}

void Fast3D::AddTri(str name, const Tri& tri, str rule)
{
	triArr += tri;
}

void Fast3D::CopyFrom(const Fast3D& prev)
{
	pnts = prev.pnts;
	for (int inx = 0; inx < pnts.size(); inx++)
	{
		pnts[inx].isBreakPnt = false;
	}
	triArr = prev.triArr;
}

str Fast3D::InfoString(int precision)
{
	str re = "";
	for (auto& p : pnts)
	{
		re += p.InfoString(precision);
	}
	return re;
}

str Fast3D::InfoString(str filter, int precision)
{
	str re = "";
	for (auto& p : pnts)
	{
		re += p.InfoString(filter, precision);
		re += " ";
	}
	return re;
}

str Fast3D::PntInfo(const str& name, const str& attrib)
{
	for (auto& p : pnts)
	{
		if (p.name == name)
		{
			return p.InfoString(attrib);
		}
	}
}
//### Fast3D