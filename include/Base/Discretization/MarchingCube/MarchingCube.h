#include "MarchingCubeTable.h"
#include "FastGrid.h"
#include "FastGeo.h"

//houdini可视化后有时会出现破洞（大部分情况下破洞占比很少或没有）
//查了很久也不知道为啥，不知道是原始算法的原因还是我哪里有诡异的bug（按理应该没有），还是houdini显示有bug

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