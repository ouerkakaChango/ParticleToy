#include "Evolver.h"

void PhysicSolver::Solve(Pnt& oldPnt, Pnt& prevPnt, Pnt& newPnt, double dt)
{
	//verlet
	P a = g;
	newPnt.pos = prevPnt.pos + newPnt.damp * (prevPnt.pos - oldPnt.pos) + a*dt*dt;
}

void PhysicSolver::SolveBegin(Pnt& prevPnt, Pnt& newPnt, double dt)
{
	//verlet
	P a = g;
	Pnt oldPnt = prevPnt;
	oldPnt.pos = prevPnt.pos + 0.5 *a *dt*dt;
	Solve(oldPnt, prevPnt, newPnt, dt);
}

void Evolver::EvolveBegin(Fast3D& prev, Fast3D& next, double dt)
{
	auto& pnts = next.pnts;
	pnts = prev.pnts;

	for (int inx = 0; inx < pnts.size(); inx++)
	{
		auto& prevPnt = prev.pnts[inx];
		auto& pnt = pnts[inx];
		if (pnt.rule.Has("PhysicProp"))
		{
			physic.SolveBegin(prevPnt, pnt, dt);
		}
	}
}

void Evolver::Evolve(Fast3D& old, Fast3D& prev, Fast3D& next, double dt)
{
	auto& pnts = next.pnts;
	pnts = prev.pnts;

	for (int inx = 0; inx < pnts.size(); inx++)
	{
		auto& oldPnt = old.pnts[inx];
		auto& prevPnt = prev.pnts[inx];
		auto& pnt = pnts[inx];
		if (pnt.rule.Has("PhysicProp"))
		{
			physic.Solve(oldPnt, prevPnt, pnt, dt);
		}
	}
}