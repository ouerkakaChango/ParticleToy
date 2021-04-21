#include "Evolver.h"

#include "EffectSpace.h"

//### ExtraInfo
ExtraInfo ExtraInfo::empty;
//### ExtraInfo

//### PhysicSolver
void PhysicSolver::Solve(const Pnt& oldPnt, const Pnt& prevPnt, Pnt& newPnt, double dt, ExtraInfo& info)
{
	P a = A(newPnt, info);

	//verlet pos
	newPnt.pos = prevPnt.pos + newPnt.damp * (prevPnt.pos - oldPnt.pos) + a*dt*dt;
	//verlet v
	newPnt.v += a * dt;
	newPnt.a = a;

	//update effectSpace
	newPnt.effectSpace.SafeUpdate(prevPnt.pos, newPnt.pos);

	insForces.clear();
}

void PhysicSolver::SolveBegin(const Pnt& prevPnt, Pnt& newPnt, double dt, ExtraInfo& info)
{
	//verlet begin: virtual oldPnt
	Pnt oldPnt = prevPnt;
	oldPnt.pos -= dt * prevPnt.v;
	Solve(oldPnt, prevPnt, newPnt, dt, info);
}

P PhysicSolver::A(const Pnt& pnt, ExtraInfo info)
{
	P finalForce = g;
	for (int inx = 0;inx<insForces.size();inx++)
	{
		const P& f = insForces[inx];
		finalForce += f;
	}
	if (pnt.rule.Has("Space"))
	{
		finalForce += UniversalG(pnt, info);
	}
	return finalForce / pnt.mass;
}

P PhysicSolver::UniversalG(const Pnt& pnt, ExtraInfo info)
{
	P re;
	for (int inx = 0; inx < spaceInxs.size(); inx++)
	{
		auto& p2 = (*info.prevPnts)[inx];
		if (info.pntInx != spaceInxs[inx])
		{
			re += norm(p2.pos - pnt.pos) * G * p2.mass * pnt.mass / dis2(p2.pos, pnt.pos);
		}
	}

	return re;
}

void PhysicSolver::InitSpace()
{
	g = P(0, 0, 0);
}
//### PhysicSolver

//### CollisionSolver
void CollisionSolver::Load(const arr<Tri>* triArr_)
{
	triArr = triArr_;
}

void CollisionSolver::Solve(const Pnt& prev, Pnt& newPnt, double dt)
{
	for (int inx = 0; inx < triArr->size(); inx++)
	{
		 auto& tri = (*triArr)[inx];
		 auto interInfo = newPnt.effectSpace.Intersect(tri);
		 if (interInfo.hit)
		 {
			 auto hitP = interInfo.hitP;
			 //1-2.
			 double dtc=-1.0;//delta time until collision
			 double x1=0.0, x2=0.0;
			 SolveQuadra(prev.a / 2, prev.v, prev.pos - hitP,x1,x2);
			 if (x1 * x2>0)
			 {//好像不可能存在这种情况
				 abort();
			 }
			 //选择正值
			 dtc = x1;
			 if (x1 < 0)
			 {
				 dtc = x2;
			 }
			 if (dtc >= dt)
			 {//好像不可能存在这种情况
				 abort();
			 }
			 //3.
			 P v2 = prev.v + prev.a*dtc;
			 P v3 = reflect(v2, tri.n)*bounceDamp;
			 //4.
			 double dtr = dt - dtc;
			 newPnt.pos = hitP + (v3*dtr + 0.5*newPnt.a*dtr*dtr);
			 newPnt.v = v3 + newPnt.a*dtr;
			 newPnt.effectSpace.Update(newPnt.pos);
			 //5.Setup breakPnt
			 Pnt breakPnt(hitP);
			 breakPnt.a = prev.a;
			 breakPnt.v = v3;
			 newPnt.SetBreakPoint(breakPnt,dtr);		
		 }
	}
}
//### CollisionSolver

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

//### Evolver
void Evolver::EvolveBegin(const Fast3D& prev, Fast3D& next, double dt)
{
	next.CopyFrom(prev);
	//!!!
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
	for (auto& delInx : colliMerge.delArr)
	{
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