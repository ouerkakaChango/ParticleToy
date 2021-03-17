#include "Spaces.h"
#include "Utility.h"

#include <iostream>

Time::Time()
{
	auto ti = new TimeI;
	i += ti;
	r += new TimeR(this);
}

void TimeR::SayI()
{
	auto ti = Cast<TimeI*>(y->i[0]);
	std::cout << " " << ti->ii;
}

Space3D::Space3D()
{
	auto ti = new Space3DI;
	i += ti;
	r += new Space3DR(this);
}

void Space3DR::SayI()
{
	auto ti = Cast<Space3DI*>(y->i[0]);
	std::cout << " " << ti->ii[0] << " " << ti->ii[1] << " " << ti->ii[2];
}

MinkowskiSpace::MinkowskiSpace()
{
	//Set I
	for (auto obj : Time::i)
	{
		i += obj;
	}

	for (auto obj : Space3D::i)
	{
		i += obj;
	}

	//Set R
	r += new MinkowskiSpaceR(this);
	for (auto obj : Time::r)
	{
		r += obj;
	}

	for (auto obj : Space3D::r)
	{
		r += obj;
	}
}

void MinkowskiSpaceR::SayI()
{
	for (int inx = 1; inx < y->r.size(); inx++)
	{
		y->r[inx]->SayI();
	}
}