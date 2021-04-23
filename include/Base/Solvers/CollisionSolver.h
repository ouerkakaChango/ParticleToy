#pragma once

#include "SolverUtility.h"

class CollisionSolver
{
public:
	void Load(const arr<Tri>* triArr_);
	void Solve(const Pnt& prev, Pnt& newPnt, double dt);

	const arr<Tri>* triArr;
	double bounceDamp = 0.8;
};