#include "FastGrid.h"

//template <class DataClass>
//void FastGrid<DataClass>::SetSize(int edgeNum, double cellLength)
//{
//	SetSize(edgeNum, edgeNum, cellLength, cellLength);
//}

//template <class DataClass>
//void FastGrid<DataClass>::SetSize(int edgeX, int edgeY, double cellX, double cellY)
//{
//	pnts.resize(edgeX+1, edgeY+1);
//	double xStart = -edgeX / 2.0*cellX;
//	double yStart = -edgeY / 2.0*cellY;
//	//从下到上，从左向右
//	for (int j = 0; j < edgeY+1; j++)
//	{
//		for (int i = 0; i < edgeX +1; i++)
//		{
//			P2 p2d(xStart + i * cellX, yStart + j * cellY);
//			pnts[i][j] = p2d;
//		}
//	}
//}