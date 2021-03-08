#include "Spaces.h"

#include <iostream>

Time::Time()
{
	auto ti = new TimeI;
	i += ti;
	r += new TimeR(ti);
}

TimeR::TimeR(TimeI* i_):i(i_)
{

}

void TimeR::SayI()
{
	std::cout << " " << i->ii;
}

Space3D::Space3D()
{
	auto ti = new Space3DI;
	i += ti;
	r += new Space3DR(ti);
}

Space3DR::Space3DR(Space3DI* i_):i(i_)
{

}

void Space3DR::SayI()
{
	std::cout << " " << i->ii[0] << " " << i->ii[1] << " " << i->ii[2];
}

MinkowskiSpace::MinkowskiSpace()
{
	for (auto obj : Time::i)
	{
		i += obj;
	}

	for (auto obj : Space3D::i)
	{
		i += obj;
	}

	for (auto obj : Time::r)
	{
		r += obj;
	}

	for (auto obj : Space3D::r)
	{
		r += obj;
	}
}
