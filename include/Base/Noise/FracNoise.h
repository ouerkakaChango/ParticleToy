#pragma once

#include "Noise.h"

#include "FastMath.h"

class FracNoise : public Noise
{
	THISY(FracNoise)

	//arbitrary numbers
	P seed_dir = P(0.63, 0.76, 0.476);
	double seed_wScale = 243758.5453123;

	double Get(const P& p);
};

double fracNoise(const P& p);