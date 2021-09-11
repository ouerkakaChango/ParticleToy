#pragma once
#include "Noise.h"

#include "FastMath.h"
#include "FracNoise.h"
#include <functional>
using std::function;

P defaultSmoothFunc(const P& p);

class CellNoise :public Noise
{
	THISY(CellNoise)

	double Get(const P& st);

	function<double(const P& p)> cornerFunc = fracNoise;
	function<P(const P& p)> smoothFunc = defaultSmoothFunc;
};

double cellNoise(const P& p);