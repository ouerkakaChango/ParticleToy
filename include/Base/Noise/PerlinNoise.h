#pragma once
#include "Noise.h"

#include "FastGrid.h"
#include "Tools/RandomStream.h"
//²Î¿¼https://blog.csdn.net/u010669231/article/details/97051705

class PerlinNoise : public Noise
{
public:
	PerlinNoise(FastGrid3D<double>* grid,P resolution_=P(25,25,25));			//auto noise resolution


	RandomStream rs_grad;
	P resolution;
protected:
	void CreateGradGrid(FastGrid3D<double>* dataGrid);
};

class PerlinNoiseI : public NoiseI
{
public:

};

template<class T>
class PerlinNoiseI3D : public PerlinNoiseI
{
public:
	FastGrid3D<T>* grid = nullptr;
};