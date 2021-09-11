#pragma once

#include "CellNoise.h"

class FBM : public Noise
{
	THISY(FBM)
	double Get(P p);

	function<double(const P& p)> noiseFunc = cellNoise;
	int octave = 6;
	double maxHeight = 1.0;
	double lacunarity = 2.0;
	double gain = 0.5;
};