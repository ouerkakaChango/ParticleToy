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

	SetDataByGrad();
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
}

template<>
void PerlinNoiseI3D<double>::SetDataByGrad()
{
	GRID_PosFunc(func_setNoise, double)
	{
		//1.by pos, GetCellData from grad
		arr<P> cellPnts;
		arr<P> cellDatas;
		grad.getCellDataByPos(pntPos, cellPnts, cellDatas);
		P uvw = uvInCell(pntPos, cellPnts);
		arr<double> nums;
		UnitCell3D<P> cell(cellDatas);
		GRID_PosFunc(func_calcuInCell, P)
		{
			//2.将结果保存在x分量中
			nums += dot(data,uvw - pntPos);
		};
		cell.DoByPos(func_calcuInCell);
		uvw = smooth5(uvw);
		//3.见2，所以Interp X分量
		data = lerp3D(nums, uvw);

	};
	data->DoByPos(func_setNoise);
}
//### PerlinNoiseI3D