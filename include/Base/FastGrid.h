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

	void Set(const P2& inx, DataClass data)
	{
		if (inx.x < 0 || inx.y < 0 || inx.x >= datas.x || inx.y >= datas.y)
		{
			abort();
			return;
		}
		bool bXMid = inx.x - inx.IntX() != 0;
		bool bYMid = inx.y - inx.IntY() != 0;
		if (bXMid || bYMid)
		{
			int ix = inx.IntX(), iy = inx.IntY();
			if (bXMid && !bYMid)
			{
				datas[ix][iy] = data;
				datas[ix + 1][iy] = data;
			}
			else if (!bXMid && bYMid)
			{
				datas[ix][iy] = data;
				datas[ix][iy+1] = data;
			}
			else
			{
				datas[ix][iy] = data;
				datas[ix + 1][iy] = data;
				datas[ix + 1][iy + 1] = data;
				datas[ix][iy + 1] = data;
			}
		}
		else
		{
			datas[inx.IntX()][inx.IntY()] = data;
		}
	}

	DataClass Get(const P2& inx)
	{
		return datas[inx.IntX()][inx.IntY()];
	}

	arr2<P2> pnts;
	arr2<DataClass> datas;
};