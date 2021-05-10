#pragma once
#include "FastMath.h"

template<class DataClass>
class FastGrid
{
public:
	void SetSize(int edgeNum, double cellLength)
	{
		SetSize(edgeNum, edgeNum, cellLength, cellLength);
	}

	void SetSize(int edgeX, int edgeY, double cellX, double cellY)
	{
		pnts.resize(edgeX + 1, edgeY + 1);
		datas.resize(edgeX + 1, edgeY + 1);
		double xStart = -edgeX / 2.0*cellX;
		double yStart = -edgeY / 2.0*cellY;
		//从下到上，从左向右
		for (int j = 0; j < edgeY + 1; j++)
		{
			for (int i = 0; i < edgeX + 1; i++)
			{
				P2 p2d(xStart + i * cellX, yStart + j * cellY);
				pnts[i][j] = p2d;
			}
		}
	}

	void Mix(int x, int y, const DataClass& data)
	{
		auto ori = datas[x][y];
		if (zero(ori))
		{
			datas[x][y] = data;
		}
		else
		{
			datas[x][y] = (ori + data) / 2;
		}
	}

	void Set(const P2& inx, const DataClass& data)
	{
		if (inx.x < 0 || inx.y < 0 || inx.x >= datas.x || inx.y >= datas.y)
		{
			abort();
			return;
		}

		int ix = inx.IntX(), iy = inx.IntY();
		double fx = inx.x - ix;
		double fy = inx.y - iy;

		bool doublex = equal(fx, 0.5);
		bool doubley = equal(fy, 0.5);

		if (!doublex && !doubley)
		{
			Mix(inx.RoundX(), inx.RoundY(),data);
		}
		else if (doublex && !doubley)
		{
			Mix(ix, inx.RoundY(), data);
			Mix(ix + 1, inx.RoundY(), data);
		}
		else if (!doublex && doubley)
		{
			Mix(inx.RoundX(), iy, data);
			Mix(inx.RoundX(), iy + 1, data);
		}
		else
		{
			Mix(ix, iy, data);
			Mix(ix, iy + 1, data);
			Mix(ix + 1, iy, data);
			Mix(ix + 1, iy + 1, data);
		}
	}

	DataClass Get(const P2& inx)
	{
		DataClass re;
		if (inx.x < 0 || inx.y < 0 || inx.x >= datas.x || inx.y >= datas.y)
		{
			abort();
			return re;
		}
		int ix = inx.IntX(), iy = inx.IntY();
		double fx = inx.x - ix;
		double fy = inx.y - iy;

		bool doublex = equal(fx, 0.5);
		bool doubley = equal(fy, 0.5);

		if (!doublex && !doubley)
		{
			return datas[inx.RoundX()][inx.RoundY()];
		}
		else if (doublex && !doubley)
		{
			re = datas[ix][inx.RoundY()];
			re += datas[ix + 1][inx.RoundY()];
			return re / 2;
		}
		else if (!doublex && doubley)
		{
			re = datas[inx.RoundX()][iy];
			re += datas[inx.RoundX()][iy + 1];
			return re / 2;
		}
		else
		{
			re = datas[ix][iy];
			re += datas[ix][iy + 1];
			re += datas[ix + 1][iy];
			re += datas[ix + 1][iy + 1];
			return re / 4;
		}
	}

	arr2<P2> pnts;
	arr2<DataClass> datas;
};

class Tri;
void FastGridToTri(const FastGrid<double>& grid, arr<Tri>& triArr);

template<class DataClass>
class FastGrid3D
{
public:
	void SetSize(int edgeX, int edgeY, int edgeZ, P cell, const DataClass& defaultData)
	{
		pnts.resize(edgeX + 1, edgeY + 1, edgeZ + 1);
		datas.resize(edgeX + 1, edgeY + 1, edgeZ + 1);

		double xStart = 0;
		double yStart = 0;
		double zStart = 0;
		for (int k=0; k < edgeZ + 1; k++)
		{
			for (int j = 0; j < edgeY + 1; j++)
			{
				for (int i = 0; i < edgeX + 1; i++)
				{
					P pos(xStart + i * cell.x, yStart + j * cell.y, zStart + k * cell.z);
					pnts[i][j][k] = pos;
					datas[i][j][k] = defaultData;
				}
			}
		}
	}

	void SetSize(int edgeX, int edgeY, int edgeZ, double cellLength, const DataClass& defaultData)
	{
		SetSize(edgeX, edgeY, edgeZ, P(cellLength, cellLength, cellLength), defaultData);
	}

	inline int Inx(int i, int j, int k)
	{
		return i  + j * pnts.y + k * pnts.x * pnts.y;
	}

	void DoByPos(std::function<void(DataClass& data, P pntPos, int pntInx)> func)
	{
		for (int k = 0; k < pnts.z; k++)
		{
			for (int j = 0; j < pnts.y; j++)
			{
				for (int i = 0; i < pnts.x; i++)
				{
					func(datas[i][j][k], pnts[i][j][k], Inx(i,j,k));
				}
			}
		}
	}

	void DoByInx(std::function<void(DataClass& data)> func)
	{
		for (int k = 0; k < pnts.z; k++)
		{
			for (int j = 0; j < pnts.y; j++)
			{
				for (int i = 0; i < pnts.x; i++)
				{
					int inx = Inx(i, j, k);
					func(datas[i][j][k]);
				}
			}
		}
	}

	void DoByNearest(std::function<void(DataClass& data, P dir, int otherInx)> func)
	{
		for (int k = 0; k < pnts.z; k++)
		{
			for (int j = 0; j < pnts.y; j++)
			{
				for (int i = 0; i < pnts.x; i++)
				{
					DataClass& data = datas[i][j][k];
					//x dir
					if (i - 1 >= 0)
					{
						func(data, P(-1, 0, 0), Inx(i - 1, j, k));
					}
					if (i + 1 < pnts.x)
					{
						func(data, P(1, 0, 0), Inx(i + 1, j, k));
					}
					//y dir
					if (j - 1 >= 0)
					{
						func(data, P(0, -1, 0), Inx(i, j - 1, k));
					}
					if (j + 1 < pnts.y)
					{
						func(data, P(0, 1, 0), Inx(i, j + 1, k));
					}
					//z dir
					if (k - 1 >= 0)
					{
						func(data, P(0, 0, -1), Inx(i, j, k - 1));
					}
					if (k + 1 < pnts.z)
					{
						func(data, P(0, 0, 1), Inx(i, j, k + 1));
					}
				}
			}
		}
	}

	arr3<P> pnts;
	arr3<DataClass> datas;
};