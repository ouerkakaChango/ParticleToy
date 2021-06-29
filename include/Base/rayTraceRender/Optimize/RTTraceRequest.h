#pragma once
#include "Taiji.h"
#include "rayTraceRender/rayTraceUtility.h"

class rayTraceWorld;
class rayTraceScreen;
class TraceRay;
//i:request序列化
//o:request输入处理
class RTTraceRequest
{
THISY(RTTraceRequest)
	void InitData(rayTraceWorld* world, rayTraceScreen* screen);
	void SetRequest(int i, int j, TraceRay& ray);
	void PrepareForNext();
};

class RTTraceRequestData : public ClassO
{
public:
	//virtual void InitData(rayTraceWorld* world) = 0;
	virtual void SetRequest(int i, int j, TraceRay& ray)=0;
	virtual void PrepareForNext()=0;
};

struct RTTraceReq
{
	P ori, dir;
};

class RTTraceRequestData_SDFSphere : public RTTraceRequestData
{
public:
	void InitData(rayTraceWorld* world_, rayTraceScreen* screen);
	void SetRequest(int i, int j, TraceRay& ray) override;
	void PrepareForNext() override;

	arr<arr2<RTTraceReq>> data;
	rayTraceWorld* world;
	int w, h;
};