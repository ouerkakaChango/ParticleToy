#pragma once

#include "SolverUtility.h"

class PhysicSolver
{
public:
	void Solve(const Pnt& oldPnt, const Pnt& prevPnt, Pnt& newPnt, double dt, ExtraInfo& info = ExtraInfo::empty);
	void SolveBegin(const Pnt& prevPnt, Pnt& newPnt, double dt, ExtraInfo& info = ExtraInfo::empty);
	P A(const Pnt& pnt, ExtraInfo info);
	P UniversalG(const Pnt& pnt, ExtraInfo info);
	void InitSpace();

	//Ĭ��ΪUE4��-980
	P g = P(0.0, 0.0, -980.0);
	double G = 1.0; //???
	arr<P> insForces;
	arr<int> spaceInxs;
};