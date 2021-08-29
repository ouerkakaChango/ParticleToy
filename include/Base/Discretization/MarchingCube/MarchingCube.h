#include "MarchingCubeTable.h"
#include "FastGrid.h"
#include "FastGeo.h"

//��ԭʼ��MarchingCube�㷨����ʱ������ƶ����󲿷�������ƶ�ռ�Ⱥ��ٻ�û�У�
//google ��Marching Cube ambiguity

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