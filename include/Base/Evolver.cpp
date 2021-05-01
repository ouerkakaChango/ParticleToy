#include "Evolver.h"

#include "EffectSpace.h"

//### Evolver
void Evolver::EvolveBegin(const Fast3D& prev, Fast3D& next, double dt)
{
	next.CopyFrom(prev);

	InitCollision(prev);
	InitSpace(prev);
	InitColliMerge(next);

	//### Solve
	auto& pnts = next.pnts;
	for (int inx = 0; inx < pnts.size(); inx++)
	{
		SolvePntBegin(inx, prev.pnts, pnts, dt);
	}
}

void Evolver::Evolve(const Fast3D& old, const Fast3D& prev, Fast3D& next, double dt)
{
	next.CopyFrom(prev);

	InitCollision(prev);
	InitSpace(prev);
	InitColliMerge(prev);

	//### Solve
	auto& pnts = next.pnts;
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
	else if (pnt.rule.Has("Molecule"))
	{
		ExtraInfo info;
		info.pntInx = inx;
		info.prevPnts = &prevPnts;
		physic.SolveBegin(prevPnt, pnt, dt, info);
		collision.Solve(prevPnt, pnt, dt);
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
			auto oldPnt = collision.GetVirtualOldPnt(prevPnt, dt);
			physic.Solve(oldPnt, prevPnt, pnt, dt);
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
	else if (pnt.rule.Has("Molecule"))
	{
		ExtraInfo info;
		info.pntInx = inx;
		info.prevPnts = &prevPnts;

		if (prevPnt.IsBreakPoint())
		{
			auto oldPnt = collision.GetVirtualOldPnt(prevPnt, dt);
			physic.Solve(oldPnt, prevPnt, pnt, dt, info);
		}
		else
		{
			physic.Solve(oldPnt, prevPnt, pnt, dt, info);
		}
		collision.Solve(prevPnt, pnt, dt);
	}
}

void Evolver::InitSpace(const Fast3D& prev)
{
	auto& pnts = prev.pnts;
	bool hasInitedSpace = false;
	physic.uniGInxs.clear();
	for (int inx = 0; inx < pnts.size(); inx++)
	{
		auto& pnt = pnts[inx];
		if (!hasInitedSpace && pnt.rule.Has("Space"))
		{
			physic.InitSpace();
			hasInitedSpace = true;
		}
		if (physic.IsCalcuUniversalG(pnt))
		{
			physic.uniGInxs += inx;
		}
	}
}

void Evolver::InitColliMerge(const Fast3D& prev)
{
	auto& pnts = prev.pnts;
	colliMerge.Clear();
	for (int inx = 0; inx < pnts.size(); inx++)
	{
		auto& pnt = pnts[inx];
		if (pnt.rule.Has("ColliMerge"))
		{
			//注意加了一个"d"
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

void Evolver::InitCollision(const Fast3D& prev)
{
	auto& pnts = prev.pnts;
	collision.Clear();
	collision.Load(&prev.triArr);
	for (int inx = 0; inx < pnts.size(); inx++)
	{
		auto& pnt = pnts[inx];
		if (RuleOf(pnt.rule,"Collision"))
		{
			collision.pntArr += inx;
		}
	}
}
//### Evolver