#pragma once
#include "Spaces.h"

class Grid : public EuclideanSpace
{
	THISY(Grid)
	template<class DataClass>
	void SetGridSettings(int edgeNum, double cellLength)
	{
		auto ti = new GridI<DataClass>;
		i += ti;
		auto& grid = ti->grid;
		grid.SetSize(edgeNum, cellLength);
	}

	template<class DataClass>
	void SetGridSettings(int edgeX, int edgeY, double cellLength, const DataClass& defaultData)
	{
		auto ti = new GridI<DataClass>;
		i += ti;
		auto& grid = ti->grid;
		grid.SetSize(edgeX, edgeY, cellLength, cellLength);
	}

	template<class DataClass>
	void SetGrid3DSettings(int edgeX, int edgeY, int edgeZ, double cellLength, const DataClass& defaultData)
	{
		auto ti = new Grid3DI<DataClass>;
		i += ti;
		auto& grid = ti->grid;
		grid.SetSize(edgeX, edgeY, edgeZ, cellLength, defaultData);
	}
};

template <class DataClass>
class GridI : public ClassI
{
public:
	FastGrid<DataClass> grid;
};

template <class DataClass>
class Grid3DI : public ClassI
{
public:
	FastGrid3D<DataClass> grid;
};

class GridO : public ClassO
{
public:

};

class GridR : public R
{
	THISR(Grid)
	void EasyTerrain(double initH, double roughness, int detailLevel = 100);
	void TerrainToTri(arr<Tri>& triArr);
	void DebugSay(int mode = 1);
	void DebugOutput(const str& filePath);
};