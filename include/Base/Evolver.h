#pragma once

#include "Fast3D.h"

class PhysicSolver
{
public:
	void Solve(Pnt& oldPnt, Pnt& prevPnt, Pnt& newPnt, double dt);
	void SolveBegin(Pnt& prevPnt, Pnt& newPnt, double dt);
	//Ä¬ÈÏÎªUE4µÄ-980
	P g=P(0.0,0.0,-980.0);
};

class Evolver
{
public:
	void EvolveBegin(Fast3D& prev, Fast3D& next, double dt);
	void Evolve(Fast3D& old, Fast3D& prev, Fast3D& next, double dt);
	PhysicSolver physic;
};