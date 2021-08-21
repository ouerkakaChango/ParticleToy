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
			arr<P> edgePnts = GetEdgePnts(cellPnts);
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
				inside[i] = 1;
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
			P p3 = cellPnts[i + 4];
			P p4 = cellPnts[(i + 1) % 4 + 4];
			re += (p1 + p2)*0.5;
			re += (p1 + p3)*0.5;
			re += (p3 + p4)*0.5;
		}
		return re;
	}
	//### MarchingCube
}