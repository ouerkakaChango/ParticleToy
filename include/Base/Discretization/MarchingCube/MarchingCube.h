#include "MarchingCubeTable.h"
#include "FastGrid.h"
#include "FastGeo.h"

namespace Discretization
{
	class MarchingCube
	{
	public:
		//MarchingCube() {};
		void March(FastGrid3D<double>& grid);
		void SetSurface(double surface_);

		int GetCutID(const arr<double*>& cellWeights);
		arr<P> GetEdgePnts(const arr<P>& cellPnts);

		arr<Tri> triArr;
		double surface=0.0;
	};
}