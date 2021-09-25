// ParticleToy.cpp : ���ļ����� "main" ����������ִ�н��ڴ˴���ʼ��������
//
#ifdef MOLECULE

#include "pch.h"
#include <iostream>

#include "../include/Base/Spaces/Spaces.h"

//### Ӧ��
//ParticleToy������Ӧ�ã����Ӽ������ڻ����������飬����...��
//1.�����Ӻ͵���ʵ�ִ��뾶��ײ
//2.�������ӣ���ƽ�ض�����������������Ӱ�죬��Ч����Ӧ���ǻ�������һ�𣨴���ײ����
//3.����restPosition���������յ�����ƫ��restPos�ĳ��������Ƿ���ʵ�����Ʒ��Ӽ���Ч����������������
//����һ������
//1.1 ��д�������������ཻ�ж�
//4.���ڱȽ�hardcode,�Ȼ�������ô�������restForce�����Գ������
//5.relation���ɻ���restPos���ٿ�һ��Ч��
//6.�����µ�restPos force�����������徧���ȶ��ԣ���˼���Ƿ���������̫ǿ��/����̫Զ����ԭ�ӵķ��Ӽ����ѣ���ģ������/�ڻ���
//7.??? ע��pnt�ڸ��Ƶ�ʱ��outer�ĸ��ƣ�������û��

int main()
{
	MinkowskiSpace* world = new MinkowskiSpace;
	world->SetFrameSettings(361, 0.0166666);
	MinkowskiSpaceR* op = (MinkowskiSpaceR*)world->r[0];
	op->SetGravity(P(0.0f, -9.80665, 0.0f));

	//Pnt atom1(P(0.0f,1.0f,0.0f));
	//atom1.name = "atom1";
	//atom1.rule = "Molecule";
	//atom1.SetSphereOuter(1.0f);
	//atom1.relations += new RestPosRelation(1, P(4, 0, 0),20);
	//atom1.relations += new RestPosRelation(2, P(0, 4, 0),100);
	//op->PutPnt(atom1);
	//
	//Pnt atom2(P(4.0f, 1.0f, 0.0f));
	//atom2.name = "atom2";
	//atom2.rule = "Molecule";
	//atom2.SetSphereOuter(1.0f);
	//atom2.relations += new RestPosRelation(0, P(-4, 0, 0),20);
	//op->PutPnt(atom2);
	//
	//Pnt atom3(P(0.0f, 5.0f, 0.0f));
	//atom3.name = "atom3";
	//atom3.rule = "Molecule";
	//atom3.SetSphereOuter(1.0f);
	//atom3.relations += new RestPosRelation(0, P(0, -4, 0), 100);
	//op->PutPnt(atom3);
	//
	//Pnt atom4(P(4.0f, 5.0f, 0.0f));
	//atom4.name = "atom4";
	//atom4.rule = "Molecule";
	//atom4.SetSphereOuter(1.0f);
	//atom4.relations += new RestPosRelation(1, P(0, -4, 0), 100);
	//op->PutPnt(atom4);

	Grid* cellGrid = new Grid;
	Pnt templatePnt;
	templatePnt.rule = "Molecule";
	templatePnt.SetSphereOuter(1.0f);
	cellGrid->SetGrid3DSettings<Pnt>(1, 1, 1, 4.0f, templatePnt);
	auto& grid3d = Cast<Grid3DI<Pnt>*>(cellGrid->i[0])->grid;
	auto func = [](Pnt& pnt, P pntPos, int pntInx)
	{
		pnt.pos = pntPos + P(0, 1, 0);
		pnt.name = str("atom");
		pnt.name += pntInx;
	};
	grid3d.DoByPos(func);
	auto func2 = [](Pnt& pnt, P pntPos, int pntInx)
	{
		std::cout << pnt.name << " " << pnt.rule << " " << pnt.pos.ToStr() << std::endl;
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
	//��ÿ��ԭ����Χ����relation
	grid3d.DoByNearest(relationFunc);

	auto putPntFunc = [&](Pnt& pnt)
	{
		op->PutPnt(pnt);
	};
	grid3d.DoByInx(putPntFunc);

	Grid* terrain = new Grid;
	terrain->SetGridSettings<float>(2, 30.0f);
	arr<Tri> triArr;
	GridR* op2 = (GridR*)terrain->r[0];

	//op2->DebugOutput("C:/HoudiniProjects/PToyScene/atomPlane.txt");

	op2->TerrainToTri(triArr);
	op->PutTri("terrain", triArr, "CollisionProp");

	auto f1 = [&](TickInfo& info)
	{
		PhysicSolver* physic = info.physic;
		auto F = info.F;
		float nowRate = pow(1 - F / 180.0f, 1);
		physic->RelationRate = nowRate;
	};
	op->AddBeforeTickFunc(f1);

	op->Evolve(0);
	//op->DebugSay();
	op->OutputPntTrajTxt("C:/HoudiniProjects/PToyScene/golfBall.txt");


	return 0;
}

#endif