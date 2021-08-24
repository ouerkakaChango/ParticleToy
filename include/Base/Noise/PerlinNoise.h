#pragma once
#include "Noise.h"
#include "Spaces/Grid.h"

class PerlinNoise : public Noise
{
public:
	PerlinNoise(Grid* grid_);

};

class PerlinNoiseI : public NoiseI
{
public:
	Grid* grid = nullptr;
};

template<class T>
class PerlinNoiseI3D : public PerlinNoiseI
{
public:
	FastGrid3D<T>* grid = nullptr;
};