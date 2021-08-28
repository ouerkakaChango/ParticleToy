#include "FastGrid.h"

#include "FastGeo.h"

//### Global FastGrid
void FastGridToTri(const FastGrid<double>& grid, arr<Tri>& triArr)
{
	auto& datas = grid.datas;
	for (int j = 0; j < datas.y-1; j++)
	{
		for (int i = 0; i < datas.x-1; i++)
		{
			//1 cell,2tri
			P p1(grid.pnts[i][j], "zx");
			p1.y = grid.datas[i][j];
			P p2(grid.pnts[i+1][j], "zx");
			p2.y = grid.datas[i+1][j];
			P p3(grid.pnts[i+1][j+1], "zx");
			p3.y = grid.datas[i+1][j+1];
			P p4(grid.pnts[i][j+1], "zx");
			p4.y = grid.datas[i][j+1];

			triArr += Tri(p1, p2, p3);
			triArr += Tri(p3, p4, p1);
		}
	}
}
//### Global FastGrid

//### global grid Utility
P uvInCell(P pos, const arr<P>& cellPos)
{
	//����FastGrid3D��ķ��أ���1����С�㣬��7���Ǵ��
	P cellSize = cellPos[6] - cellPos[0];
	return (pos - cellPos[0]) / cellSize;
}
//### global grid Utility