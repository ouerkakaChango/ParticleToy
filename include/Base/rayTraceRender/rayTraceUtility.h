#pragma once
#include "FastMath.h"

class Object;

enum rayTraceMode
{
	rayTraceMode_SDFSphere,
};

enum rayTraceBounceMode
{
	rayTraceBounceMode_cheap,	//仅使用单方向(trace反射方向)作为IndirectLight，通过材质reflectness混合反射
	rayTraceBounceMode_reflect,	//和cheap几乎一样，但将indirect点作为点光源进入光照模型计算，混合更物理
	rayTraceBounceMode_MonteCarlo, //经典蒙特卡洛路径追踪，也就是只考虑2trace多条采样（比如spp32）,而不是n*n。也算是reflect升级版。
};

enum rayTraceMaterialMode
{
	rayTraceMaterialMode_BlinnPhong,
	rayTraceMaterialMode_PBR,
};

class TraceInfo
{
public:
	bool bHit = false;
	P dir;
	P hitPos;
	P hitN;
	double dis = 0.0;
	Object* obj = nullptr;

	P color;
};
