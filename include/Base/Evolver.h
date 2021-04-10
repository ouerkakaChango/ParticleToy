#pragma once

#include "Fast3D.h"

class PhysicSolver
{
public:
	void Solve(const Pnt& oldPnt, const Pnt& prevPnt, Pnt& newPnt, double dt);
	void SolveBegin(const Pnt& prevPnt, Pnt& newPnt, double dt);
	P A();
	//Ĭ��ΪUE4��-980
	P g=P(0.0,0.0,-980.0);
	arr<P> insForces;
	double m=1.0;
};

class CollisionSolver
{
public:
	void Load(const arr<Tri>* triArr_);
	void Solve(const Pnt& prev, Pnt& newPnt, double dt);

	const arr<Tri>* triArr;
	double bounceDamp = 0.8;
};

class Evolver
{
public:
	void EvolveBegin(const Fast3D& prev, Fast3D& next, double dt);
	void Evolve(const Fast3D& old, const Fast3D& prev, Fast3D& next, double dt);
	PhysicSolver physic;
	CollisionSolver collision;
};