#pragma once

#include "SolverUtility.h"

//存在意义：因为各种原因，所以不在physicSolver里精确算支持力，而用几何的办法
//强改位置，并使用一定的物理公式，所以是"Physic Based Collision"，但不Physic

class CollisionSolver
{
public:
	void Load(const arr<Tri>* triArr_);
	void Solve(const Pnt& prev, Pnt& newPnt, double dt);
	Pnt GetVirtualOldPnt(const Pnt& prev, double dt) const;
	const Tri& GetTri(int uid) const;

	const arr<Tri>* triArr;
	double bounceDamp = 0.8;
};