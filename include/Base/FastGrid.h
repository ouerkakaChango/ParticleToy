#pragma once
#include "FastMath.h"

class FastGrid
{
public:
	void SetSize(int edgeNum, double cellLength);
	void SetSize(int edgeX, int edgeY, double cellX, double cellY);
	arr2<P> pnts;
};