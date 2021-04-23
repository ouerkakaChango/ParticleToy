#pragma once

#include "Fast3D.h"
#include "Solvers/PhysicSolver.h"
#include "Solvers/CollisionSolver.h"
#include "Solvers/ColliMergeSolver.h"

class Evolver
{
public:
	void EvolveBegin(const Fast3D& prev, Fast3D& next, double dt);
	void Evolve(const Fast3D& old, const Fast3D& prev, Fast3D& next, double dt);
	void InitSpace(const Fast3D& prev);
	void InitColliMerge(Fast3D& next);
	void SolvePntBegin(int inx, const arr<Pnt>& prevPnts, arr<Pnt>& pnts, double dt);
	void SolvePnt(int inx, const arr<Pnt>& oldPnts, const arr<Pnt>& prevPnts, arr<Pnt>& pnts, double dt);
	void Clear(arr<Pnt>& pnts);

	PhysicSolver physic;
	CollisionSolver collision;
	ColliMergeSolver colliMerge;
};