#include "PerlinNoise.h"

#include "NoiseUtility.h"

#include <iostream>
using std::cout; 
using std::endl;

//### PerlinNoise
PerlinNoise::PerlinNoise(FastGrid3D<double>* grid, P resolution_)
	:resolution(resolution_)
{
	auto ti = new PerlinNoiseI3D<double>;
	ti->grid = grid;
	i += ti;

	rs_grad.Init(&PerlinGrad3D_12);

	CreateGradGrid(grid);
}

void PerlinNoise::CreateGradGrid(FastGrid3D<double>* dataGrid)
{
	P size = dataGrid->cellSize * dataGrid->subDivide;
	cout << "Create gradGrid size:"+size.ToStr()+" ,resolution:"+resolution.ToStr();
}

//### PerlinNoise