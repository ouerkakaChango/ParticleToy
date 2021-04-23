#pragma once

#include "../Container.h"
#include "../FastPnt.h"

class ExtraInfo
{
public:
	int pntInx = -1;
	int colliMergeMode = -1;//0:toMerge,1:merged
	static ExtraInfo empty;
	const arr<Pnt> * prevPnts = nullptr;
	arr<Pnt> * newPnts = nullptr;
};