#include "Evolver.h"

#include "EffectSpace.h"

//### Evolver
void Evolver::EvolveBegin(const Fast3D& prev, Fast3D& next, double dt)
{
	next.CopyFrom(prev);
	collision.Load(&prev.triArr);

	auto& pnts = next.pnts;

	InitSpace(prev);
	InitColliMerge(next);

	//### Solve
	for (int inx = 0; inx < pnts.size(); inx++)
	{
		SolvePntBegin(inx, prev.pnts, pnts, dt);
	}
}

void Evolver::Evolve(const Fast3D& old, const Fast3D& prev, Fast3D& next, double dt)
{
	next.CopyFrom(prev);
	collision.Load(&prev.triArr);

	auto& pnts = next.pnts;

	InitSpace(prev);
	InitColliMerge(next);

	//### Solve
	for (int inx = 0; inx < pnts.size(); inx++)
	{
		if (inx >= old.pnts.size())
		{
			if (inx >= prev.pnts.size())
			{
				//don't solve newly added pnt now,solveBegin it at next frame
			}
			else
			{
				SolvePntBegin(inx, prev.pnts, pnts, dt);
			}
		}
		else
		{
			SolvePnt(inx, old.pnts, prev.pnts, pnts, dt);
		}
	}
	//### Solve colliMerge
	for (int inx = 0; inx < pnts.size(); inx++)
	{
		auto& pnt = pnts[inx];
		if (pnt.rule.Has("ColliMerge"))
		{
			ExtraInfo info;
			info.pntInx = inx;
			info.colliMergeMode = pnt.rule.Has("ColliMerged") ? 1 : 0;
			info.newPnts = &pnts;
			colliMerge.Solve(info);
		}
	}

	Clear(pnts);
}

void Evolver::Clear(arr<Pnt>& pnts)
{
	for (int i = 0; i < pnts.size(); i++)
	{
		pnts[i].inxOffset = 0;
	}

	for (auto& delInx : colliMerge.delArr)
	{
		for (int i = delInx + 1; i < pnts.size(); i++)
		{
			pnts[i].inxOffset += 1;
		}
	}

	for (int i = colliMerge.delArr.size() - 1; i >= 0; i--)
	{
		int delInx = colliMerge.delArr[i];
		pnts.delAt(delInx);
	}
}

void Evolver::SolvePntBegin(int inx, const arr<Pnt>& prevPnts, arr<Pnt>& pnts, double dt)
{
	const Pnt& prevPnt = prevPnts[inx];
	auto& pnt = pnts[inx];
	if (pnt.rule.Has("PhysicProp"))
	{
		physic.SolveBegin(prevPnt, pnt, dt);
		collision.Solve(prevPnt, pnt, dt);
	}
	else if (pnt.rule.Has("Space"))
	{
		ExtraInfo info;
		info.pntInx = inx;
		info.prevPnts = &prevPnts;
		physic.SolveBegin(prevPnt, pnt, dt, info);
	}
}

void Evolver::SolvePnt(int inx, const arr<Pnt>& oldPnts, const arr<Pnt>& prevPnts, arr<Pnt>& pnts, double dt)
{
	auto& pnt = pnts[inx];
	auto& oldPnt = oldPnts[inx + pnt.inxOffset];
	auto& prevPnt = prevPnts[inx];
	if (pnt.rule.Has("PhysicProp"))
	{
		if (prevPnt.IsBreakPoint())
		{
			physic.Solve(prevPnt.GetVirtualOldPnt(dt), prevPnt, pnt, dt);
		}
		else
		{
			physic.Solve(oldPnt, prevPnt, pnt, dt);
		}
		collision.Solve(prevPnt, pnt, dt);
	}
	else if (pnt.rule.Has("Space"))
	{
		ExtraInfo info;
		info.pntInx = inx;
		info.prevPnts = &prevPnts;
		physic.Solve(oldPnt, prevPnt, pnt, dt, info);
	}
}

void Evolver::InitSpace(const Fast3D& prev)
{
	auto& pnts = prev.pnts;
	bool initSpace = true;
	physic.spaceInxs.clear();
	for (int inx = 0; inx < pnts.size(); inx++)
	{
		auto& pnt = pnts[inx];
		if (pnt.rule.Has("Space"))
		{
			if (initSpace)
			{
				physic.InitSpace();
				initSpace = false;
			}
			physic.spaceInxs += inx;
		}
	}
}

void Evolver::InitColliMerge(Fast3D& next)
{
	auto& pnts = next.pnts;
	bool initColliMerge = true;
	colliMerge.Clear();
	for (int inx = 0; inx < pnts.size(); inx++)
	{
		auto& pnt = pnts[inx];
		if (pnt.rule.Has("ColliMerge"))
		{
			if (pnt.rule.Has("ColliMerged"))
			{
				colliMerge.mergedArr += inx;
			}
			else
			{
				colliMerge.toMergeArr += inx;
			}
		}
	}
}
//### Evolver