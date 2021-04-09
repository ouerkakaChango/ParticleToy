#include "Evolver.h"

#include "EffectSpace.h"

void PhysicSolver::Solve(const Pnt& oldPnt, const Pnt& prevPnt, Pnt& newPnt, double dt)
{
	P a = g;

	//verlet pos
	newPnt.pos = prevPnt.pos + newPnt.damp * (prevPnt.pos - oldPnt.pos) + a*dt*dt;
	//verlet v
	newPnt.v += a * dt;
	newPnt.a = a;

	//update effectSpace
	newPnt.effectSpace.SafeUpdate(prevPnt.pos, newPnt.pos);
}

void PhysicSolver::SolveBegin(const Pnt& prevPnt, Pnt& newPnt, double dt)
{
	//verlet
	P a = g;
	Pnt oldPnt = prevPnt;
	oldPnt.pos = prevPnt.pos + 0.5 *a *dt*dt;
	Solve(oldPnt, prevPnt, newPnt, dt);
}

void CollisionSolver::Load(const arr<Tri>* triArr_)
{
	triArr = triArr_;
}

void CollisionSolver::Solve(const Pnt& prev, Pnt& newPnt, double dt)
{
	for (int inx = 0; inx < triArr->size(); inx++)
	{
		 auto& tri = (*triArr)[0];
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

void Evolver::EvolveBegin(const Fast3D& prev, Fast3D& next, double dt)
{
	next.CopyFrom(prev);
	collision.Load(&prev.triArr);

	auto& pnts = next.pnts;

	for (int inx = 0; inx < pnts.size(); inx++)
	{
		const Pnt& prevPnt = prev.pnts[inx];
		auto& pnt = pnts[inx];
		if (pnt.rule.Has("PhysicProp"))
		{
			physic.SolveBegin(prevPnt, pnt, dt);
			collision.Solve(prevPnt, pnt, dt);
		}
	}
}

void Evolver::Evolve(const Fast3D& old, const Fast3D& prev, Fast3D& next, double dt)
{
	next.CopyFrom(prev);

	auto& pnts = next.pnts;

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
	}
}