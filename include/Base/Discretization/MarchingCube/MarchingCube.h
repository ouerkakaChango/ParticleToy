#include "MarchingCubeTable.h"
#include "FastGrid.h"
#include "FastGeo.h"

//最原始的MarchingCube算法，有时会出现破洞（大部分情况下破洞占比很少或没有）
//google 搜Marching Cube ambiguity

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
		arr<P> GetEdgePntsByWeight(const arr<P>& cellPnts, const arr<double*>& cellWeights);
		void TestAll();


		arr<Tri> triArr;
		double surface=0.0;
	};
}