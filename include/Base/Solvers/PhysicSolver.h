#pragma once

#include "SolverUtility.h"

class PhysicSolver
{
public:
	void Solve(const Pnt& oldPnt, const Pnt& prevPnt, Pnt& newPnt, double dt, ExtraInfo& info);
	void SolveBegin(const Pnt& prevPnt, Pnt& newPnt, double dt, ExtraInfo& info);
	P A(const Pnt& pnt, ExtraInfo info);
	P UniversalG(const Pnt& pnt, ExtraInfo info);
	P RestForce(const Pnt& pnt, ExtraInfo info);
	void InitSpace();

	//默认为UE4的-980
	P g = P(0.0, 0.0, -980.0);
	double G = 1.0; //!!! 万有引力常数
	arr<P> insForces;
	arr<int> uniGInxs;

	double RelationRate = 1.0;
};