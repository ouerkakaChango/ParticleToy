#pragma once
#include "FastMath.h"

#define RAYTRACE_OpenMP
#ifdef RAYTRACE_OpenMP
#include <omp.h>
#include <thread>
#include <mutex>
#endif 
class Object;

enum rayTraceMode
{
	rayTraceMode_SDFSphere,
};

enum rayTraceBounceMode
{
	rayTraceBounceMode_cheap,	//��ʹ�õ�����(trace���䷽��)��ΪIndirectLight��ͨ������reflectness��Ϸ���
	rayTraceBounceMode_reflect,	//��cheap����һ��������indirect����Ϊ���Դ�������ģ�ͼ��㣬��ϸ�����
	rayTraceBounceMode_MonteCarlo, //�������ؿ���·��׷�٣�Ҳ����ֻ����2trace��������������spp32��,������n*n��Ҳ����reflect�����档
};

enum rayTraceMaterialMode
{
	rayTraceMaterialMode_BlinnPhong,
	rayTraceMaterialMode_PBR,
};

enum rayTraceOptimizeMode
{
	rayTraceOptimizeMode_None,
	rayTraceOptimizeMode_PerTask,	//��һ��������w*h��ray������ÿn��ray bounce��������д������Ȼ����һ��
	rayTraceOptimizeMode_NumbaCUDA,
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

class ClassRayTraceGod
{
public:
	static ClassRayTraceGod& GetInstance();
#ifdef RAYTRACE_OpenMP
	std::mutex mtx;
#endif
	str optimizeWorkPath;
	str pythonWorkPath;
};
#define rayTraceGod ClassRayTraceGod::GetInstance()