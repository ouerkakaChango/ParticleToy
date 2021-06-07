#pragma once
#include "FastMath.h"

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
