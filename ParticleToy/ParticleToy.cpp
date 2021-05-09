// ParticleToy.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>

#include "../include/Base/Spaces.h"

//### 应用
//ParticleToy第三期应用：分子键（冰融化，刚体破碎，柔体...）
//1.让粒子和地面实现带半径碰撞
//2.两个粒子，在平地都受重力和万有引力影响，看效果（应该是互相吸到一起（带碰撞））
//3.引入restPosition，让粒子收到基于偏移restPos的斥力，看是否能实现类似分子键的效果，让两个粒子能
//保持一定距离
//1.1 重写胶囊与三角面相交判定
//4.现在比较hardcode,等会想想怎么把这个算restForce的属性抽象出来
//5.relation换成基于restPos的再看一下效果
//6.基于新的restPos force，测试立方体晶格稳定性，并思考是否让受外力太强的/距离太远的两原子的分子键断裂，以模拟破碎/融化？

int main()
{
	MinkowskiSpace* world = new MinkowskiSpace;
	world->SetFrameSettings(1001, 0.0166666);
	MinkowskiSpaceR* op = (MinkowskiSpaceR*)world->r[0];
	op->SetGravity(P(0.0, -9.80665, 0.0));

	Pnt atom1(P(0.0,1.0,0.0));
	atom1.name = "atom1";
	atom1.rule = "Molecule";
	atom1.SetSphereOuter(1.0);
	atom1.relations += new RestPosRelation(1, P(4, 0, 0),10);
	atom1.relations += new RestPosRelation(2, P(0, 4, 0),100);
	op->PutPnt(atom1);

	Pnt atom2(P(3.0, 5.0, 0.0));
	atom2.name = "atom2";
	atom2.rule = "Molecule";
	atom2.SetSphereOuter(1.0);
	atom2.relations += new RestPosRelation(0, P(-4, 0, 0),10);
	op->PutPnt(atom2);

	Pnt atom3(P(0.0, 5.0, 0.0));
	atom3.name = "atom3";
	atom3.rule = "Molecule";
	atom3.SetSphereOuter(1.0);
	atom3.relations += new RestPosRelation(0, P(0, -4, 0), 100);
	op->PutPnt(atom3);

	Grid* terrain = new Grid;
	terrain->SetGridSettings<double>(2, 30.0);
	arr<Tri> triArr;
	GridR* op2 = (GridR*)terrain->r[0];

	//op2->DebugOutput("C:/HoudiniProjects/PToyScene/atomPlane.txt");

	op2->TerrainToTri(triArr);
	op->PutTri("terrain", triArr, "CollisionProp");

	op->Evolve(0);
	//op->DebugSay();
	op->OutputPntTrajTxt("C:/HoudiniProjects/PToyScene/golfBall.txt");


	return 0;
}