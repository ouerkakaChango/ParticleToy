#include "MarchingCube.h"

#include <iostream>
using std::cout;
using std::endl;

namespace Discretization
{
	//### MarchingCube
	void MarchingCube::March(FastGrid3D<double>& grid)
	{
		GRID_CellFunc(func,double)
		{
			int cutID = GetCutID(cellDatas);
			//arr<P> edgePnts = GetEdgePnts(cellPnts);
			arr<P> edgePnts = GetEdgePntsByWeight(cellPnts,cellDatas);

			int i = 0;
			while (i<15-2-1 && triangulation[cutID][i] != -1)
			{
				int i1 = triangulation[cutID][i];
				int i2 = triangulation[cutID][i + 1];
				int i3 = triangulation[cutID][i + 2];
				P p1 = edgePnts[i1];
				P p2 = edgePnts[i2];
				P p3 = edgePnts[i3];
				triArr += Tri(p1, p2, p3);
				i += 3;
			}
		};
		grid.DoByCell(func);
	}

	void MarchingCube::SetSurface(double surface_)
	{
		surface = surface_;
	}

	int MarchingCube::GetCutID(const arr<double*>& cellWeights)
	{
		int inside[8] = {};
		for (int i = 0; i < 8; i++)
		{
			if (*cellWeights[i] >= surface)
			{
				inside[7-i] = 1;
			}
		}

		return ToDecimal(inside,8);
	}

	arr<P> MarchingCube::GetEdgePnts(const arr<P>& cellPnts)
	{
		arr<P> re;
		for (int i = 0; i < 4; i++)
		{
			P p1 = cellPnts[i];
			P p2 = cellPnts[(i + 1) % 4];
			re += (p1 + p2)*0.5;
		}
		for (int i = 0; i < 4; i++)
		{
			P p3 = cellPnts[i + 4];
			P p4 = cellPnts[(i + 1) % 4 + 4];
			re += (p3 + p4)*0.5;
		}
		for (int i = 0; i < 4; i++)
		{
			P p1 = cellPnts[i];
			P p3 = cellPnts[i + 4];
			re += (p1 + p3)*0.5;
		}
		return re;
	}

	arr<P> MarchingCube::GetEdgePntsByWeight(const arr<P>& cellPnts, const arr<double*>& cellWeights)
	{
		arr<P> re;
		for (int i = 0; i < 4; i++)
		{
			P p1 = cellPnts[i];
			P p2 = cellPnts[(i + 1) % 4];
			double w1 = *cellWeights[i];
			double w2 = *cellWeights[(i + 1) % 4];
			double k = 0.5;
			if ((w1 - surface)*(w2 - surface) < 0)
			{
				k = lerpRate(w1, w2, surface);
			}
			k = clamp(k, 0.01, 0.99);
			re += lerp(p1,p2, k);
		}
		for (int i = 0; i < 4; i++)
		{
			P p3 = cellPnts[i + 4];
			P p4 = cellPnts[(i + 1) % 4 + 4];
			double w3 = *cellWeights[i + 4];
			double w4 = *cellWeights[(i + 1) % 4 + 4];
			double k = 0.5;
			if ((w3 - surface)*(w4 - surface) < 0)
			{
				k = lerpRate(w3, w4, surface);
			}
			k = clamp(k, 0.01, 0.99);
			re += lerp(p3, p4, k);
		}
		for (int i = 0; i < 4; i++)
		{
			P p1 = cellPnts[i];
			P p3 = cellPnts[i + 4];
			double w1 = *cellWeights[i];
			double w3 = *cellWeights[i + 4];
			double k = 0.5;
			if ((w1 - surface)*(w3 - surface) < 0)
			{
				k = lerpRate(w1, w3, surface);
			}
			k = clamp(k, 0.01, 0.99);
			re += lerp(p1, p3, k);
		}
		return re;
	}

	void MarchingCube::TestAll()
	{
		arr<double> a = { 0,0,0,0,0,0,0,0 };
		UnitCell3D<double> cell(a);
		auto edgePnts = GetEdgePnts(cell.pnts);

		for (int inx = 0; inx < 256; inx++)
		{

			int i = 0;
			while (i < 15 - 2 - 1 && triangulation[inx][i] != -1)
			{
				int i1 = triangulation[inx][i];
				int i2 = triangulation[inx][i + 1];
				int i3 = triangulation[inx][i + 2];
				P p1 = edgePnts[i1];
				P p2 = edgePnts[i2];
				P p3 = edgePnts[i3];
				triArr += Tri(p1, p2, p3);
				i += 3;
			}

			for (auto& p : edgePnts)
			{
				p += P(2, 0, 0);
			}
		}
	}
	//### MarchingCube
}