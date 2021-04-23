#include "ColliMergeSolver.h"

//### ColliMergeSolver
void ColliMergeSolver::Clear()
{
	toMergeArr.clear();
	mergedArr.clear();
	delArr.clear();
}

void ColliMergeSolver::Solve(ExtraInfo& info)
{
	int inx1 = -1, inx2 = -1;
	auto& pnts = *info.newPnts;
	if (delArr.Has(info.pntInx))
	{
		return;
	}
	if (info.colliMergeMode == 0)
	{
		inx1 = info.pntInx;
		//遍历mergedArr，如果第二点不被标记删除且两点碰撞，则SolveColi
		//并标记toMergeInx已被删除，但不立即删除，因为会有inx相关麻烦
		//在Evolver的Solve中后面一并删除
		for (int i = 0; i < mergedArr.size(); i++)
		{
			inx2 = mergedArr[i];
			if (!delArr.Has(inx2) && pnts[inx1].Collide(pnts[inx2]))
			{
				ColliMerge(pnts[inx1], pnts[inx2]);
				delArr += inx1;
			}
		}
	}
	else if (info.colliMergeMode == 1)
	{
		inx2 = info.pntInx;
		for (int i = 0; i < toMergeArr.size(); i++)
		{
			inx1 = toMergeArr[i];
			if (!delArr.Has(inx1) && pnts[inx2].Collide(pnts[inx1]))
			{
				ColliMerge(pnts[inx1], pnts[inx2]);
				delArr += inx1;
			}
		}
	}
	else
	{
		abort();
	}
}

void ColliMergeSolver::ColliMerge(Pnt& p1, Pnt& p2)
{
	//动量守恒 m1v1+m2v2=m3v3 --> v3=(m1v1+m2v2)/(m1+m2)
	double m1 = p1.mass, m2 = p2.mass;
	P v1 = p1.v, v2 = p2.v;
	p2.mass = m1 + m2;
	p2.v = (m1*v1 + m2 * v2) / (m1 + m2);
}
//### ColliMergeSolver