#include "PhysicSolver.h"

//### PhysicSolver
void PhysicSolver::Solve(const Pnt& oldPnt, const Pnt& prevPnt, Pnt& newPnt, double dt, ExtraInfo& info)
{
	P a = A(newPnt, info);

	//verlet pos
	newPnt.pos = prevPnt.pos + newPnt.damp * (prevPnt.pos - oldPnt.pos) + a * dt*dt;
	//verlet v
	newPnt.v += a * dt;
	newPnt.a = a;

	//update effectSpace
	newPnt.effectSpace.SafeUpdate(prevPnt, newPnt);

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
	for (int inx = 0; inx < insForces.size(); inx++)
	{
		const P& f = insForces[inx];
		finalForce += f;
	}
	if (IsCalcuUniversalG(pnt))
	{
		finalForce += UniversalG(pnt, info);
	}
	return finalForce / pnt.mass;
}

P PhysicSolver::UniversalG(const Pnt& pnt, ExtraInfo info)
{
	P re;
	for (int inx = 0; inx < uniGInxs.size(); inx++)
	{
		auto& p2 = (*info.prevPnts)[inx];
		if (info.pntInx != uniGInxs[inx])
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

bool PhysicSolver::IsCalcuUniversalG(const Pnt& pnt)
{
	str rule = pnt.rule;
	return rule.Has("Space") || rule.Has("Molecule");
}
//### PhysicSolver