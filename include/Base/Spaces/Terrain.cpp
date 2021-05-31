#include "Terrain.h"

//### EasyTerrainAlgo
void EasyTerrainAlgo::Create(GridI<double>* terrain, int detailLevel_, double initH_, double roughness_)
{
	if (detailLevel_ <= 0 || roughness_ < 0)
	{
		abort();
		return;
	}
	if (terrain == nullptr)
	{
		abort();
		return;
	}
	detailLevel = detailLevel_;
	initH = initH_;
	roughness = roughness_;
	grid = &terrain->grid;

	InitCorner(initH_ / 2.0);

	bool bLoop = true;
	int detailInx = 0;
	while (bLoop)
	{
		stepSize = (grid->datas.x - 1) / std::pow(2, detailInx);
		Square();
		if (bMaxFrac)
		{
			break;
		}
		Diamond();
		if (bMaxFrac)
		{
			break;
		}
		detailInx += 1;
		if (detailInx == detailLevel)
		{
			break;
		}
	}
}

void EasyTerrainAlgo::InitCorner(double h)
{
	int stepSize = grid->datas.x - 1;
	P2 inx1 = P2(0, 0);
	P2 inx2 = P2(0, 0) + P2(1, 0)*stepSize;
	P2 inx3 = P2(0, 0) + P2(1, 1)*stepSize;
	P2 inx4 = P2(0, 0) + P2(0, 1)*stepSize;

	grid->Set(inx1, h);
	grid->Set(inx2, h);
	grid->Set(inx3, h);
	grid->Set(inx4, h);
}

double EasyTerrainAlgo::Offset()
{
	return roughness * stepSize * (2 * rand01() - 1)* initH;
}

double EasyTerrainAlgo::H(double h1, double h2, double h3, double h4)
{
	return (h1 + h2 + h3 + h4) / 4 + Offset();
}

void EasyTerrainAlgo::Square()
{
	if (stepSize < 2)
	{
		bMaxFrac = true;
		return;
	}
	P2 inxStart(0, 0);
	int edgeNum = Cast<int>((grid->datas.x - 1) / stepSize);
	for (int j = 0; j < edgeNum; j++)
	{
		for (int i = 0; i < edgeNum; i++)
		{
			P2 inx1 = inxStart;
			P2 inx2 = inxStart + P2(1, 0)*stepSize;
			P2 inx3 = inxStart + P2(1, 1)*stepSize;
			P2 inx4 = inxStart + P2(0, 1)*stepSize;
			P2 inxCenter = inxStart + P2(0.5, 0.5)*stepSize;

			double h1 = grid->Get(inx1);
			double h2 = grid->Get(inx2);
			double h3 = grid->Get(inx3);
			double h4 = grid->Get(inx4);
			double h = H(h1, h2, h3, h4);
			grid->Set(inxCenter, h);
			inxStart += P2(1, 0)*stepSize;
		}
		inxStart.x = 0;
		inxStart.y += stepSize;
	}
}

void EasyTerrainAlgo::Diamond()
{
	if (stepSize < 2)
	{
		bMaxFrac = true;
		return;
	}
	P2 inxStart(0, 0);
	int edgeNum = Cast<int>((grid->datas.x - 1) / stepSize);
	for (int j = 0; j < edgeNum; j++)
	{
		for (int i = 0; i < edgeNum; i++)
		{
			P2 inx1 = inxStart;
			P2 inx2 = inxStart + P2(1, 0)*stepSize;
			P2 inx3 = inxStart + P2(1, 1)*stepSize;
			P2 inx4 = inxStart + P2(0, 1)*stepSize;
			P2 inxCenter = inxStart + P2(0.5, 0.5)*stepSize;

			double h1 = grid->Get(inx1);
			double h2 = grid->Get(inx2);
			double h3 = grid->Get(inx3);
			double h4 = grid->Get(inx4);
			double hc = grid->Get(inxCenter);

			//dia1:1-2-c
			SubDiamond(inx1, inx2, inxCenter, h1, h2, hc);

			//dia2:2-3-c
			SubDiamond(inx2, inx3, inxCenter, h2, h3, hc);

			//dia3:3-4-c
			SubDiamond(inx3, inx4, inxCenter, h3, h4, hc);

			//dia4:4-1-c
			SubDiamond(inx4, inx1, inxCenter, h4, h1, hc);

			inxStart += P2(1, 0)*stepSize;
		}
		inxStart.x = 0;
		inxStart.y += stepSize;
	}
}

void EasyTerrainAlgo::SubDiamond(const P2& inx1, const P2& inx2, const P2& inxCenter,
	double h1, double h2, double hc)
{
	P2 inxC1 = (inx1 + inx2) / 2;
	P2 inxO1 = inxC1 * 2 - inxCenter;
	if (inxO1 >= P2(0, 0) && inxO1 < P2(grid->datas.x, grid->datas.y))
	{
		double ho = grid->Get(inxO1);
		grid->Set(inxC1, H(h1, h2, hc, ho));
	}
	else
	{
		grid->Set(inxC1, H(h1, h2, hc));
	}
}
//### EasyTerrainAlgo