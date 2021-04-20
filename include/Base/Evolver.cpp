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
		auto& p2 = (*info.pnts)[inx];
		if (info.pntNum != spaceInxs[inx])
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
}
//### ColliMergeSolver

//### Evolver
void Evolver::EvolveBegin(const Fast3D& prev, Fast3D& next, double dt)
{
	next.CopyFrom(prev);
	//!!!
	collision.Load(&prev.triArr);

	auto& pnts = next.pnts;

	InitSpace(next);
	InitColliMerge(next);

	//### Solve
	for (int inx = 0; inx < pnts.size(); inx++)
	{
		const Pnt& prevPnt = prev.pnts[inx];
		auto& pnt = pnts[inx];
		if (pnt.rule.Has("PhysicProp"))
		{
			physic.SolveBegin(prevPnt, pnt, dt);
			collision.Solve(prevPnt, pnt, dt);
		}
		else if (pnt.rule.Has("Space"))
		{
			ExtraInfo info;
			info.pntNum = inx;
			info.pnts = &prev.pnts;
			physic.SolveBegin(prevPnt, pnt, dt, info);
		}
	}
}

void Evolver::Evolve(const Fast3D& old, const Fast3D& prev, Fast3D& next, double dt)
{
	next.CopyFrom(prev);

	auto& pnts = next.pnts;

	InitSpace(next);
	InitColliMerge(next);

	//### Solve
	for (int inx = 0; inx < pnts.size(); inx++)
	{
		auto& oldPnt = old.pnts[inx];
		auto& prevPnt = prev.pnts[inx];
		auto& pnt = pnts[inx];
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
			info.pntNum = inx;
			info.pnts = &prev.pnts;
			physic.Solve(oldPnt, prevPnt, pnt, dt, info);
		}
		//if (pnt.rule.Has("ColliMerge"))
		//{
		//	if (pnt.rule.Has("ColliMerged"))
		//	{
		//
		//	}
		//}
	}
}

void Evolver::InitSpace(Fast3D& next)
{
	auto& pnts = next.pnts;
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