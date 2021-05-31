#include "Grid.h"

class TerrainAlgo : public GridO
{
public:
};

class EasyTerrainAlgo : public TerrainAlgo
{
public:
	void Create(GridI<double>* terrain, int detailLevel_, double initH_, double roughness_);
	void InitCorner(double h);
	void Square();
	void Diamond();
	void SubDiamond(const P2& inx1, const P2& inx2, const P2& inxCenter,
		double h1, double h2, double hc);
	double Offset();
	double H(double h1, double h2, double h3, double h4 = 0.0);

	FastGrid<double>* grid;
	int detailLevel;
	double initH;
	bool bMaxFrac = false;
	double stepSize = -10000.0;
	double roughness = 0.3;
};