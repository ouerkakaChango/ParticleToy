#pragma once

#include "Fast3D.h"

class ExtraInfo
{
public:
	int pntInx = -1;
	int colliMergeMode = -1;//0:toMerge,1:merged
	static ExtraInfo empty;
	const arr<Pnt> * prevPnts = nullptr;
	arr<Pnt> * newPnts = nullptr;
};

class PhysicSolver
{
public:
	void Solve(const Pnt& oldPnt, const Pnt& prevPnt, Pnt& newPnt, double dt, ExtraInfo& info = ExtraInfo::empty);
	void SolveBegin(const Pnt& prevPnt, Pnt& newPnt, double dt, ExtraInfo& info = ExtraInfo::empty);
	P A(const Pnt& pnt, ExtraInfo info);
	P UniversalG(const Pnt& pnt, ExtraInfo info);
	void InitSpace();

	//Ä¬ÈÏÎªUE4µÄ-980
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

class ColliMergeSolver
{
public:
	void Clear();
	void Solve(ExtraInfo& info);
	void ColliMerge(Pnt& p1, Pnt& p2);

	arr<int> toMergeArr;
	arr<int> mergedArr;
	arr<int> delArr;
};

class Evolver
{
public:
	void EvolveBegin(const Fast3D& prev, Fast3D& next, double dt);
	void Evolve(const Fast3D& old, const Fast3D& prev, Fast3D& next, double dt);
	void InitSpace(const Fast3D& prev);
	void InitColliMerge(Fast3D& next);
	void SolvePntBegin(int inx, const arr<Pnt>& prevPnts, arr<Pnt>& pnts, double dt);
	void SolvePnt(int inx, const arr<Pnt>& oldPnts, const arr<Pnt>& prevPnts, arr<Pnt>& pnts, double dt);

	PhysicSolver physic;
	CollisionSolver collision;
	ColliMergeSolver colliMerge;
};