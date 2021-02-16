// ParticleToy.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>

#include "../include/Base/Taiji.h"

//类设计哲学:
//1.类名后加后缀，Y为太极，I为阳，O为阴，R为人。阳上有阴阳，阴上有阴阳，以此类推。
//2.按实际应用，一般来说，I,O同继承于Y。
//3.人要使用任何东西，必须通过使用R。
//4.按照本人哲学逻辑，对于使用者来说，任何Thing都很难扩展或继承，但很容易组合。
//5.用R类创建，一般来说，生阳指阴。CreateXXXI(XXXO),用阴初始化阳。

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

//1.地形
//Ying-Arrangement-Arrange3D(3DSpaceY)-Terrain3D(参数diamond displacement)
//https://www.playfuljs.com/realistic-terrain-in-130-lines/
//WorldR.Put3DPoints(Terrain3D(diamond)),由于地形比较特殊，不必要用时空点，使用（World的）基类的操作类（3DSpaceR）的函数，创建地形也行。

//2.高尔夫球质点
//阴:能量守恒 阳:表现-位移(速度，加速度)
//Particle对应的Rule初始化PhysicProp
//WorldR.CreateParticle("PhysicProp");

//3.初始击打
//照理来说，一切本已存在。但从源头考虑的话太复杂。
//需要“突然”添加一个改变因素。
//WorldR.ChangeRule(golfPoint,...);

//--操作0:准备3DSpaceR
//--操作1:Put3DPoint，参数Terrain3D，迭代1
//--操作2:Put3DPoint，参数Terrain3D，迭代2

int main()
{
    std::cout << "Hello World!\n"; 
	Yang k;
	//1.
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
