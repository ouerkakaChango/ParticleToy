#include "Grid.h"

#include <iostream>
#include <fstream>
#include "Terrain.h"

//### Grid
Grid::Grid()
{
	//Set R
	r += new GridR(this);
}
//### Grid

//### GridR
void GridR::SayI()
{
	auto& grid = Cast<GridI<double>*>(y->i[0])->grid;
	auto& pnts = grid.pnts;
	for (int j = 0; j < pnts.y; j++)
	{
		for (int i = 0; i < pnts.x; i++)
		{
			auto& p = pnts[i][j];
			std::cout << "(" << p.ToStr().data << "):" << grid.datas[i][j] << " ";
		}
		std::cout << "\n";
	}
}

void GridR::EasyTerrain(double initH, double roughness, int detailLevel)
{
	if (detailLevel == 0)
	{
		abort();
		return;
	}
	auto to = new EasyTerrainAlgo;
	y->o += to;

	auto grid = Cast<GridI<double>*>(y->i[0]);
	to->Create(grid, detailLevel, initH, roughness);
}

void GridR::TerrainToTri(arr<Tri>& triArr)
{
	auto& fGrid = Cast<GridI<double>*>(y->i[0])->grid;
	FastGridToTri(fGrid, triArr);
}

void GridR::DebugSay(int mode)
{
	auto& grid = Cast<GridI<double>*>(y->i[0])->grid;
	for (int j = 0; j < grid.datas.y; j++)
	{
		for (int i = 0; i < grid.datas.x; i++)
		{
			P p(grid.pnts[i][j], "zx");
			p.y = grid.datas[i][j];
			if (mode == 1)
			{
				std::cout << p.ToStr().data << "\n";
			}
			else if (mode == 2)
			{
				std::cout << p.ToStr().data << "--- " << P2(i, j).ToStr().data << "\n";
			}
		}
	}
}

void GridR::DebugOutput(const str& filePath)
{
	std::cout << "Writing File...\n";
	std::ofstream f(filePath.data, std::ios::out);
	auto& grid = Cast<GridI<double>*>(y->i[0])->grid;
	for (int j = 0; j < grid.datas.y; j++)
	{
		for (int i = 0; i < grid.datas.x; i++)
		{
			P p(grid.pnts[i][j], "zx");
			p.y = grid.datas[i][j];
			f << p.ToStr().data << std::endl;
		}
	}
	std::cout << "File write done at " << filePath.data << "\n";
}
//### GridR