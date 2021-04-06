#include "Evolver.h"

#include "EffectSpace.h"

void PhysicSolver::Solve(const Pnt& oldPnt, const Pnt& prevPnt, Pnt& newPnt, double dt)
{
	P a = g;

	//verlet
	newPnt.pos = prevPnt.pos + newPnt.damp * (prevPnt.pos - oldPnt.pos) + a*dt*dt;

	//update effectSpaceI
	newPnt.EffectUpdate(prevPnt);
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

void CollisionSolver::Solve(Pnt& pnt)
{
	for (int inx = 0; inx < triArr->size(); inx++)
	{
		 auto& tri = (*triArr)[0];
		 auto interInfo = pnt.effectSpace->Intersect(tri);
		 //if (interInfo.bIntersect)
		 {
			 //pnt.pos = calcuNewPos...(interInfo)
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
			collision.Solve(pnt);
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
			physic.Solve(oldPnt, prevPnt, pnt, dt);
			collision.Solve(pnt);
		}
	}
}