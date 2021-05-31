#pragma once
#include "../Spaces/Spaces.h"

class BounceTime : public Ying
{
	THISY(BounceTime)

	int bounceNum=0;
};

class photonWorld : public BounceTime, public Space3D
{
	THISY(photonWorld)
	void SetBounceSettings(int bounceNum_);
};

class photonWorldR : public R
{
	THISR(photonWorld)
};