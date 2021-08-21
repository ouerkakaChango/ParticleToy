#include "WriterUtility.h"

P TransWriteFormat(const P& pos, WriteMode mode)
{
	P p = pos;
	if (mode == WriteMode_Houdini)
	{
		double tt = p.y;
		p.y = p.z;
		p.z = tt;
	}
	return p;
}

Tri TransWriteFormat(const Tri& tri, WriteMode mode)
{
	Tri re = tri;
	if (mode == WriteMode_Houdini)
	{
		re.p1 = TransWriteFormat(re.p1, mode);
		re.p2 = TransWriteFormat(re.p2, mode);
		re.p3 = TransWriteFormat(re.p3, mode);
	}
	return re;
}