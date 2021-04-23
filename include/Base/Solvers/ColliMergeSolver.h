#pragma once

#include "SolverUtility.h"

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