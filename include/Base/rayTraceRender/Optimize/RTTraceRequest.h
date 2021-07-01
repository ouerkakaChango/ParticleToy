#pragma once
#include "Taiji.h"
#include "rayTraceRender/rayTraceUtility.h"

class rayTraceWorld;
class rayTraceScreen;
class TraceRay;

enum RequestMode
{
	RequestMode_txt,
};

//i:request序列化
//o:request输入处理
class RTTraceRequest
{
THISY(RTTraceRequest)
	void InitData(rayTraceWorld* world_, rayTraceScreen* screen, const str& outPath,RequestMode mode_= RequestMode_txt);
	void SetRequest(rayTraceScreen* screen_);
	void PrepareForNext();
	void SendAndWaitGetResult();

	RequestMode mode= RequestMode_txt;
	rayTraceScreen* screen=nullptr;
	rayTraceWorld* world = nullptr;
	int requestNum = 0;
	int taskNum = 4;
	int sendInx = 0;
};

class RTTraceRequestO : public ClassO
{
public:
	RTTraceRequestO(RTTraceRequest* y_);
	virtual ~RTTraceRequestO() {};
	//virtual void InitData(rayTraceWorld* world) = 0;
	virtual void PrepareForNext()=0;

	RTTraceRequest* y = nullptr;
};

class RTTraceRequestI : public ClassI
{
public:
	virtual ~RTTraceRequestI() {};
	virtual void SendRequest() = 0;
	virtual void SendSingleRequest(int reqInx) = 0;
	virtual void WaitForResult(int reqInx)=0;
	virtual void CallCalculation()=0;
	RTTraceRequestO* o;
};




class RTTraceRequestO_SDFSphere : public RTTraceRequestO
{
public:
	RTTraceRequestO_SDFSphere(RTTraceRequest* y_);
	void InitData(rayTraceWorld* world_, rayTraceScreen* screen);
	void PrepareForNext() override;

	rayTraceWorld* world;
	int w, h;
};

class RTTraceRequestI_txt : public RTTraceRequestI
{
public:
	void SendRequest() override;
	void SendSingleRequest(int reqInx) override;
	void WaitForResult(int reqInx) override;
	void CallCalculation() override;
	str outPath;
};