#pragma once

#include "Fast3D.h"

class ExtraInfo
{
public:
	int pntNum = -1;
	static ExtraInfo empty;
	const arr<Pnt> * pnts = nullptr;
};

class PhysicSolver
{
public:
	void Solve(const Pnt& oldPnt, const Pnt& prevPnt, Pnt& newPnt, double dt, ExtraInfo& info = ExtraInfo::empty);
	void SolveBegin(const Pnt& prevPnt, Pnt& newPnt, double dt, ExtraInfo& info = ExtraInfo::empty);
	P A(const Pnt& pnt, ExtraInfo info);
	P UniversalG(const Pnt& pnt, ExtraInfo info);
	void InitSpace();
	void AddSpacePnt(int inx);

	//Ĭ��ΪUE4��-980
	P g=P(0.0,0.0,-980.0);
	double G = 1.0; //???
	arr<P> insForces;
	arr<int> spaceInxs;
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