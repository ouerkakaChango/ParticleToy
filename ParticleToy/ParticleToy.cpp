// ParticleToy.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>

#include "../include/Base/Spaces.h"

//类设计哲学:
//1.类名后加后缀，Y为太极，I为阳，O为阴，R为人。阳上有阴阳，阴上有阴阳，以此类推。
//2.按实际应用，一般来说，I,O同继承于Y。
//3.人要使用任何东西，必须通过使用R。
//4.按照本人哲学逻辑，对于使用者来说，任何Thing都很难扩展或继承，但很容易组合。
//5.用R类创建，一般来说，生阳指阴。CreateXXXI(XXXO),用阴初始化阳。
//++
//6.无论阴阳，其本质不可用代码表述，其象表述方式千万。但是代码表述有默认方式，I类默认成员变量i，O类默认成员变量o。

//虚拟高尔夫项目：
//Yang-Space-MetricSpace-EuclideanSpace-3DSpace
//--3DSpaceO为阴：组合方式，别名3DCoordinate
//--3DSpaceI为阳：显示数据，别名3DPoint
//--3DSpaceR为人：用于操作3D空间
//--单操作Create_3DPoint_At_3DCoordinate，别名Put3DPoint，不需要arrangement
//--复操作Create_3DPoints_At_3DCoordinate，别名Put3DPoints，需要arrangement

//0.4D时空（Minkowski空间），3DSpace+Time，别名World
//Ying-TimeY
//多重继承MinkowsiSpaceY:public 3DSpaceY,public TimeY
//别名WorldY
//人操作类 WorldR
//WorldO为阴：世界规则/粒子属性，别名Rule
//WorldI为阳：粒子，别名Particle
//WorldI持有[3DPoint+Time,Rule]

//++ ??
//Time的阳是可以是时间戳？
//Time的阴是时间管理物的变化？

//1.地形
//Ying-Arrangement-Arrange3D(3DSpaceY)-Terrain3D(参数diamond displacement)
//https://www.playfuljs.com/realistic-terrain-in-130-lines/
//WorldR.Put3DPnts(Terrain3D(diamond)),由于地形比较特殊，不必要用时空点，使用（World的）基类的操作类（3DSpaceR）的函数，创建地形也行。

//2.高尔夫球质点
//阴:能量守恒 阳:表现-位移(速度，加速度)
//Particle对应的Rule初始化PhysicProp
//WorldR.PutWorldPnt("PhysicProp");

//3.初始击打
//照理来说，一切本已存在。但从源头考虑的话太复杂。
//需要“突然”添加一个改变因素。
//WorldR.ChangeRule(golfPoint,...);

//4.时空演变
//阳:变化
//阴:受变
//球变化后，有一个受变范围（比如boundingBox），然后受变范围里的物体受变

//5.特殊:三角面
//由于计算机存储空间有限，所以不能无限细分空间达到表述地面的功能
//只能通过指定顶点，然后数学计算surface碰撞
//所以，在电脑世界中，Surface也作为一个实物存在

//测试场景1：简单4顶点地面+重力小球
//WorldR.Put3DPnt(vert1);
//WorldR.Put3DPnt(vert2);
//WorldR.Put3DPnt(vert3);
//WorldR.Put3DPnt(vert4);
//WorldR.PutSurface(1,2,3);
//WorldR.PutSurface(2,3,4);
//WorldR.PutWorldPnt(golf,"PhysicProp");
//（由于WorldPnt为时空点，所以其自然有一串时空轨迹。
//在代码上，显示为其自更新，自管理）
//golf.GetRecord(0,120,"s",0.01); //120s的0.01s间隔帧数据，共12000个

/////////////////////////////////////////////////////////////
//1.不同PC同步测试
//2.代码构建步骤:重力小球，地面
////////////////////////////////////////////////////////////

//--操作0:准备3DSpaceR
//--操作1:Put3DPoint，参数Terrain3D，迭代1
//--操作2:Put3DPoint，参数Terrain3D，迭代2

//#############################
//同步测试
//1:2021.03.08 from SQ
//##############################
//最新规则改动
//1.无生阴阳的太极本物，本来规定后缀为Y，现在直接不加。
//2.由于C++命名限制，3DSpace->Space3D,以此类推
//!!!
//3.无论阴阳，其本质不可用代码表述，其象表述方式各异...需要将两者抽离，具体怎么做待定
//##############################
//Target List
//1.0 重力小球
//1.1 Yang-Space-MetricSpace-EuclideanSpace-Space3D
//1.2 Ying-Time
//1.3 Minkowski = Space3D + Time
//1.4 对Space3DI的需求分析和结构设计
//1.5 MinkowskiSpaceR创建一个FastParticle，每一帧对world进行evolve,从0帧开始到10帧结束
//1.6 Evolver-PhysicSolver-RigidSolver
//1.7 在SetFrameSetting时初始化所有Evolver；在PutPnt时候，注意到rule里有physic，然后打开之后帧的所有Evolver里的PhysicSolver
//1.8 Log动画数据（txt文件），然后UE4插件/Houdini播放动画

//1.9 MinkowskiSpaceR创建2个“概念三角面”，也就是通用的三角面。
//2.0 在PhysicSolver里检测碰撞，对于每个moveObj：
//2.0.1 获得此moveObj从old->prev的影响范围effectSpaceI
//2.0.2 对于effectSpaceI，求相交的effectSpaceO,并与之affect。实际上是对Pnt检测碰撞后，调整其位置和增加瞬时加速度（力）
//2.1 具体思路：每次verlet改变Pnt位置的时候，Pnt在SetPos时也更新其effectSpaceI，即一条线段effectLine,并在下次与effectTri求交
//2.2 调整结构：Fast3D要从上一帧copy，因为每个Frame的Fast3D都是独立的；静态collision可用指针共享
//2.3 bounce：verlet时存储v
//##############################
//100米自由落体 数据表格 (g=10)
//时间戳 高度（100-x）
//0		0
//0.1	0.05
//0.2	0.2
//0.3	0.45
//0.4	0.8
//0.5	1.25
//0.6	1.8
//0.7	2.45
//0.8	3.2
//0.9	4.05
//1		5

int main()
{
	MinkowskiSpace* world = new MinkowskiSpace;
	world->SetFrameSettings(361, 0.0166666);
	MinkowskiSpaceR* op = (MinkowskiSpaceR*)world->r[0];
	op->SetGravity(P(0.0, -9.80665, 0.0));

	op->PutPnt("golf", P(0,0,0), "PhysicProp");

	Tri tri1;
	tri1.FromGrid(1.0, "xz", true);
	tri1.Scale(100.0);
	tri1.Transform(P(0, -4, 0));
	op->PutTri("grid_up", tri1, "CollisionProp");

	op->PntInsForce(0, "golf", P(900.0, 400.0, 1000.0));

	op->Evolve(0);
	//p->SayI();
	//std::cout << "\n";
	//op->SayO();
	//std::cout << "\n";
	//op->Say(); //将所有帧数据输出
	op->DebugSay();
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
