// ParticleToy.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>

#include "../include/Base/Spaces.h"

//ParticleToy第二期应用：星球轨道
//一：双星（日月）轨迹
//1.使用MinkowskiSpace，添加两个星球（带outer的Pnt,此时两个质点的outerd都是Sphere）
//其中日的质量比月的大，所以发生碰撞时，月的质量直接给到日，速度也给到
//Pnt sun(1,0,0);
//sun.SetMass(10.0);
//sun.SetVel(0,0,1);
//sun.SetSphereOuter(1.0);
//op->PutPnt("sun", sun, "space colliMerged");
//Pnt moon(-1,0,0);
//moon.SetMass(0.1);
//moon.SetVel(0.0, 0.0, 0.2);
//moon.SetSphereOuter(0.1);
//op->PutPnt("moon", moon, "space colliMerge");

int main()
{
	MinkowskiSpace* world = new MinkowskiSpace;
	world->SetFrameSettings(361, 0.0166666);
	MinkowskiSpaceR* op = (MinkowskiSpaceR*)world->r[0];
	op->SetGravity(P(0.0, -9.80665, 0.0));

	Pnt sun(P(1,0,0));
	sun.name = "sun";
	sun.rule = "space colliMerged";
	sun.SetMass(10.0);
	sun.SetVel(P(0,0,1));
	sun.SetSphereOuter(1.0);
	op->PutPnt(sun);
	std::cout << sun.InfoString();
	return 0;
}