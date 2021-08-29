#pragma once
#include "FastMath.h"

template<class T>
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

	void Mix(int x, int y, const T& data)
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

	void Set(const P2& inx, const T& data)
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

	T Get(const P2& inx)
	{
		T re;
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

	void DoByPos(std::function<void(T& data, P2 pntPos)> func)
	{
		for (int j = 0; j < pnts.y; j++)
		{
			for (int i = 0; i < pnts.x; i++)
			{
				func(datas[i][j], pnts[i][j]);
			}
		}
	}

	arr2<P2> pnts;
	arr2<T> datas;
};

class Tri;
void FastGridToTri(const FastGrid<double>& grid, arr<Tri>& triArr);

template<class T>
class FastGrid3D
{
public:
	void SetSize(int edgeX, int edgeY, int edgeZ, P cell, const T& defaultData)
	{
		cellSize = cell;
		subDivide = P(edgeX, edgeY, edgeZ);
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

	void SetSize(int edgeX, int edgeY, int edgeZ, double cellLength, const T& defaultData)
	{
		SetSize(edgeX, edgeY, edgeZ, P(cellLength, cellLength, cellLength), defaultData);
	}

	void SetSize(P edgeNum, P cell, const T& defaultData)
	{
		SetSize(edgeNum.x, edgeNum.y, edgeNum.z, cell, defaultData);
	}

	inline int Inx(int i, int j, int k)
	{
		return i  + j * pnts.y + k * pnts.x * pnts.y;
	}

	void DoByPos(std::function<void(T& data, P pntPos, int pntInx)> func)
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

	void DoByInx(std::function<void(T& data)> func)
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

	void DoByNearest(std::function<void(T& data, P dir, int otherInx)> func)
	{
		for (int k = 0; k < pnts.z; k++)
		{
			for (int j = 0; j < pnts.y; j++)
			{
				for (int i = 0; i < pnts.x; i++)
				{
					T& data = datas[i][j][k];
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

	void DoByCell(std::function<void(const arr<P>& cellPnts, const arr<T*>& cellDatas)> func)
	{
		for (int k = 0; k < pnts.z-1; k++)
		{
			for (int j = 0; j < pnts.y-1; j++)
			{
				for (int i = 0; i < pnts.x-1; i++)
				{
					//gather cell vertices
					arr<P> cellPnts;
					GetCellPos(i, j, k, cellPnts);

					arr<T*> cellDatas;
					cellDatas += &datas[i][j][k];
					cellDatas += &datas[i + 1][j][k];
					cellDatas += &datas[i + 1][j + 1][k];
					cellDatas += &datas[i][j + 1][k];
					cellDatas += &datas[i][j][k + 1];
					cellDatas += &datas[i + 1][j][k + 1];
					cellDatas += &datas[i + 1][j + 1][k + 1];
					cellDatas += &datas[i][j + 1][k + 1];
					func(cellPnts, cellDatas);
				}
			}
		}
	}

	void getCellDataByPos(P pntPos, arr<P>& cellPnts, arr<T>& cellDatas)
	{
		
		P inx = GetCellInx(pntPos);

		cellPnts.clear();
		cellDatas.clear();

		GetCellPos(inx, cellPnts);
		GetCellData(inx, cellDatas);
	}

	P GetCellInx(P pntPos)
	{
		P startPos = pnts[0][0][0];
		P local = pntPos - startPos;
		P inx = ModInx(local, cellSize);
		
		//在计算pntPos的cellInx的时候，计算的是cell内的左下方点，
		//但是当p正好在右上方的时候，用Mod已经越过了这个cell，正好在(+1,+1,+1)的左下方点，需要-1处理
		if (equal(inx.x,subDivide.x))
		{
			inx.x -= 1;
		}
		if (equal(inx.y, subDivide.y))
		{
			inx.y -= 1;
		}
		if (equal(inx.z, subDivide.z))
		{
			inx.z -= 1;
		}
		return inx;
	}

	void GetCellPos(int i, int j, int k, arr<P>& cellPnts)
	{
		cellPnts += pnts[i][j][k];
		cellPnts += pnts[i + 1][j][k];
		cellPnts += pnts[i + 1][j + 1][k];
		cellPnts += pnts[i][j + 1][k];
		cellPnts += pnts[i][j][k + 1];
		cellPnts += pnts[i + 1][j][k + 1];
		cellPnts += pnts[i + 1][j + 1][k + 1];
		cellPnts += pnts[i][j + 1][k + 1];
	}

	void GetCellPos(P inx, arr<P>& cellPnts)
	{
		GetCellPos(inx.x, inx.y, inx.z, cellPnts);
	}

	void GetCellData(int i, int j, int k, arr<T>& cellDatas)
	{
		cellDatas += datas[i][j][k];
		cellDatas += datas[i + 1][j][k];
		cellDatas += datas[i + 1][j + 1][k];
		cellDatas += datas[i][j + 1][k];
		cellDatas += datas[i][j][k + 1];
		cellDatas += datas[i + 1][j][k + 1];
		cellDatas += datas[i + 1][j + 1][k + 1];
		cellDatas += datas[i][j + 1][k + 1];
	}

	void GetCellData(P inx, arr<T>& cellDatas)
	{
		GetCellData(inx.x, inx.y, inx.z, cellDatas);
	}

	void Centerlize()
	{
		P hGridSize = (P(pnts.x, pnts.y, pnts.z) - 1.0)*cellSize*0.5;
		for (int k = 0; k < pnts.z; k++)
		{
			for (int j = 0; j < pnts.y; j++)
			{
				for (int i = 0; i < pnts.x; i++)
				{
					pnts[i][j][k] -= hGridSize;
				}
			}
		}
	}

	arr3<P> pnts;
	arr3<T> datas;

	P cellSize;
	P subDivide;
};

#define GRID_PosFunc(func,dataType) auto func = [&](dataType& data, P pntPos, int pntInx)
#define GRID_CellFunc(func,dataType) auto func = [&](const arr<P>& cellPnts, const arr<dataType*>& cellDatas)

template<class T>
class UnitCell3D
{
public:
	UnitCell3D(const arr<T>& datas_)
	{
		pnts = {
			P(0,0,0),
			P(1,0,0),
			P(1,1,0),
			P(0,1,0),
			P(0,0,1),
			P(1,0,1),
			P(1,1,1),
			P(0,1,1)
		};
		datas = datas_;
	}

	void DoByPos(std::function<void(T& data, P pntPos, int pntInx)> func)
	{
		for (int i = 0; i < 8; i++)
		{
			func(datas[i],pnts[i],i);
		}
	}

	arr<T> datas;
	arr<P> pnts;
};

//### global grid Utility
P uvInCell(P pos, const arr<P>& cellPos);