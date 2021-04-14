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

	void SetDirectly(const P2& inx, DataClass data)
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
			if (!zero(datas[inx.RoundX()][inx.RoundY()]))
			{
				abort();
			}
			datas[inx.RoundX()][inx.RoundY()] = data;
		}
		else if (doublex && !doubley)
		{
			if (!zero(datas[ix][inx.RoundY()]))
			{
				abort();
			}
			if (!zero(datas[ix + 1][inx.RoundY()]))
			{
				abort();
			}
			datas[ix][inx.RoundY()] = data;
			datas[ix+1][inx.RoundY()] = data;
		}
		else if (!doublex && doubley)
		{
			if (!zero(datas[inx.RoundX()][iy]))
			{
				abort();
			}
			if (!zero(datas[inx.RoundX()][iy + 1]))
			{
				abort();
			}
			datas[inx.RoundX()][iy] = data;
			datas[inx.RoundX()][iy+1] = data;
		}
		else
		{
			if (!zero(datas[ix][iy]))
			{
				abort();
			}
			if (!zero(datas[ix][iy + 1]))
			{
				abort();
			}
			if (!zero(datas[ix + 1][iy]))
			{
				abort();
			}
			if (!zero(datas[ix + 1][iy + 1]))
			{
				abort();
			}
			datas[ix][iy] = data;
			datas[ix][iy + 1] = data;
			datas[ix + 1][iy] = data;
			datas[ix + 1][iy + 1] = data;
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