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
//7.??? 注意pnt在复制的时候outer的复制，现在先没管

int main()
{
	MinkowskiSpace* world = new MinkowskiSpace;
	world->SetFrameSettings(361, 0.0166666);
	MinkowskiSpaceR* op = (MinkowskiSpaceR*)world->r[0];
	op->SetGravity(P(0.0, -9.80665, 0.0));

	//Pnt atom1(P(0.0,1.0,0.0));
	//atom1.name = "atom1";
	//atom1.rule = "Molecule";
	//atom1.SetSphereOuter(1.0);
	//atom1.relations += new RestPosRelation(1, P(4, 0, 0),10);
	//atom1.relations += new RestPosRelation(2, P(0, 4, 0),100);
	//op->PutPnt(atom1);
	//
	//Pnt atom2(P(4.0, 1.0, 0.0));
	//atom2.name = "atom2";
	//atom2.rule = "Molecule";
	//atom2.SetSphereOuter(1.0);
	//atom2.relations += new RestPosRelation(0, P(-4, 0, 0),10);
	//op->PutPnt(atom2);
	//
	//Pnt atom3(P(0.0, 5.0, 0.0));
	//atom3.name = "atom3";
	//atom3.rule = "Molecule";
	//atom3.SetSphereOuter(1.0);
	//atom3.relations += new RestPosRelation(0, P(0, -4, 0), 100);
	//op->PutPnt(atom3);
	//
	//Pnt atom4(P(4.0, 5.0, 0.0));
	//atom4.name = "atom4";
	//atom4.rule = "Molecule";
	//atom4.SetSphereOuter(1.0);
	//atom4.relations += new RestPosRelation(1, P(0, -4, 0), 100);
	//op->PutPnt(atom4);

	Grid* cellGrid = new Grid;
	Pnt templatePnt;
	templatePnt.rule = "Molecule";
	templatePnt.SetSphereOuter(1.0);
	cellGrid->SetGrid3DSettings<Pnt>(1,1,1,4.0, templatePnt);
	auto& grid3d = Cast<Grid3DI<Pnt>*>(cellGrid->i[0])->grid;
	auto func = [](Pnt& pnt,P pntPos, int pntInx)
	{
		pnt.pos = pntPos+P(0,1,0);
		pnt.name = str("atom"); 
		pnt.name += pntInx;
	};
	grid3d.DoByPos(func);
	auto func2 = [](Pnt& pnt, P pntPos, int pntInx)
	{
		std::cout <<pnt.name << " " << pnt.rule<<" "<< pnt.pos.ToStr() <<std::endl;
	};
	grid3d.DoByPos(func2);

	auto relationFunc = [](Pnt& pnt, P dir, int otherInx)
	{
		if (dir.y != 0)
		{
			pnt.relations += new RestPosRelation(otherInx, 4 * dir, 20);
		}
		else
		{
			pnt.relations += new RestPosRelation(otherInx, 4 * dir, 10);
		}
	};
	//对每个原子周围设置relation
	grid3d.DoByNearest(relationFunc);

	auto putPntFunc = [&](Pnt& pnt)
	{
		op->PutPnt(pnt);
	};
	grid3d.DoByInx(putPntFunc);

	Grid* terrain = new Grid;
	terrain->SetGridSettings<double>(2, 30.0);
	arr<Tri> triArr;
	GridR* op2 = (GridR*)terrain->r[0];

	//op2->DebugOutput("C:/HoudiniProjects/PToyScene/atomPlane.txt");

	op2->TerrainToTri(triArr);
	op->PutTri("terrain", triArr, "CollisionProp");

	auto f1 = [&](TickInfo& info)
	{
		PhysicSolver* physic = info.physic;
		auto F = info.F;
		double nowRate = pow(1 - F / 180.0,1);
		physic->RelationRate = nowRate;
	};
	op->AddBeforeTickFunc(f1);

	op->Evolve(0);
	//op->DebugSay();
	op->OutputPntTrajTxt("C:/HoudiniProjects/PToyScene/golfBall.txt");


	return 0;
}