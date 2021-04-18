// ParticleToy.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>

#include "../include/Base/Spaces.h"

//### 架构回顾与前瞻
//1.为什么帧与帧之间的pnt不用指针共享，而要复制?这样增大了内存，而且增删极其麻烦
//答:因为每帧数据不一样，我宁可复制，不然可能就变成一个共享的pnt，然后有arr<Record>，实无意义。
//宁愿对共享数据进行额外的代码层优化。虽然增加了增删bug的风险，对程序员要求极高。
//权衡之下还是采用这种方案。毕竟此时的我非彼时的我。
//###
//### 应用
//ParticleToy第二期应用：星球轨道
//一：双星（日月）轨迹
//1.使用MinkowskiSpace，添加两个星球（带outer的Pnt,此时两个质点的outerd都是Sphere）
//其中日的质量比月的大，所以发生碰撞时，月的质量直接给到日，速度也给到
//2.万有引力的G，量纲的转换？

int main()
{
	MinkowskiSpace* world = new MinkowskiSpace;
	world->SetFrameSettings(361, 0.0166666);
	MinkowskiSpaceR* op = (MinkowskiSpaceR*)world->r[0];
	//??? 点的rule为space了，就不用刻意关闭g了
	op->SetGravity(P(0.0, 0.0, 0.0));

	Pnt sun(P(1,0,0));
	sun.name = "sun";
	sun.rule = "Space ColliMerged";
	sun.SetMass(10.0);
	sun.SetVel(P(0.0, 0.0, 0.2));
	sun.SetSphereOuter(1.0);
	op->PutPnt(sun);

	Pnt moon(P(-1, 0, 0));
	moon.name = "moon";
	moon.rule = "Space ColliMerge";
	moon.SetMass(0.1);
	moon.SetVel(P(0, 0, 1));
	moon.SetSphereOuter(0.1);
	op->PutPnt(moon);

	op->Evolve(0);

	//op->SayI();
	op->DebugSay();
	return 0;
}