#include "PerlinNoise.h"

#include "NoiseUtility.h"

#include <iostream>
using std::cout; 
using std::endl;

//### PerlinNoise
PerlinNoise::PerlinNoise(FastGrid3D<double>* grid, P resolution)
{
	auto ti = new PerlinNoiseI3D<double>;
	ti->data = grid;
	ti->Init(resolution);
	i += ti;
}
//### PerlinNoise


//### PerlinNoiseI3D
template<>
void PerlinNoiseI3D<double>::Init(P resolution_)
{
	resolution = resolution_;

	rs_grad.Init(&PerlinGrad3D_12);

	CreateGradGrid();
}

template<>
void PerlinNoiseI3D<double>::CreateGradGrid()
{
	P size = data->cellSize * data->subDivide;
	cout << "Create gradGrid size:" + size.ToStr() + " ,resolution:" + resolution.ToStr();
	P gCellSize = size / resolution;
	grad.SetSize(resolution, gCellSize, P(0, 0, 0));
	GRID_PosFunc(func, P)
	{
		data = rs_grad.Get();
	};
	grad.DoByPos(func);
	int aa = 1;
}
//### PerlinNoiseI3D