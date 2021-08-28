#pragma once
#include "Noise.h"

#include "FastGrid.h"
#include "Tools/RandomStream.h"
//²Î¿¼https://blog.csdn.net/u010669231/article/details/97051705

class PerlinNoise : public Noise
{
public:
	PerlinNoise(FastGrid3D<double>* grid,P resolution);
};

class PerlinNoiseI : public NoiseI
{
public:

};

template<class T>
class PerlinNoiseI3D : public PerlinNoiseI
{
public:
	void Init(P resolution_);
	void CreateGradGrid();
	void SetDataByGrad();

	P resolution;
	RandomStream<P> rs_grad;
	FastGrid3D<T>* data = nullptr;
	FastGrid3D<P> grad;
};