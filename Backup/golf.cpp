// ParticleToy.cpp : ���ļ����� "main" ����������ִ�н��ڴ˴���ʼ��������
//
#ifdef GOLF
#include "pch.h"
#include <iostream>

#include "../include/Base/Spaces/Spaces.h"

//�������ѧ:
//1.������Ӻ�׺��YΪ̫����IΪ����OΪ����RΪ�ˡ��������������������������Դ����ơ�
//2.��ʵ��Ӧ�ã�һ����˵��I,Oͬ�̳���Y��
//3.��Ҫʹ���κζ���������ͨ��ʹ��R��
//4.���ձ�����ѧ�߼�������ʹ������˵���κ�Thing��������չ��̳У�����������ϡ�
//5.��R�ഴ����һ����˵������ָ����CreateXXXI(XXXO),������ʼ������
//++
//6.�����������䱾�ʲ����ô�����������������ʽǧ�򡣵��Ǵ��������Ĭ�Ϸ�ʽ��I��Ĭ�ϳ�Ա����i��O��Ĭ�ϳ�Ա����o��

//����߶�����Ŀ��
//Yang-Space-MetricSpace-EuclideanSpace-3DSpace
//--3DSpaceOΪ������Ϸ�ʽ������3DCoordinate
//--3DSpaceIΪ������ʾ���ݣ�����3DPoint
//--3DSpaceRΪ�ˣ����ڲ���3D�ռ�
//--������Create_3DPoint_At_3DCoordinate������Put3DPoint������Ҫarrangement
//--������Create_3DPoints_At_3DCoordinate������Put3DPoints����Ҫarrangement

//0.4Dʱ�գ�Minkowski�ռ䣩��3DSpace+Time������World
//Ying-TimeY
//���ؼ̳�MinkowsiSpaceY:public 3DSpaceY,public TimeY
//����WorldY
//�˲����� WorldR
//WorldOΪ�����������/�������ԣ�����Rule
//WorldIΪ�������ӣ�����Particle
//WorldI����[3DPoint+Time,Rule]

//++ ??
//Time�����ǿ�����ʱ�����
//Time������ʱ�������ı仯��

//1.����
//Ying-Arrangement-Arrange3D(3DSpaceY)-Terrain3D(����diamond displacement)
//https://www.playfuljs.com/realistic-terrain-in-130-lines/
//WorldR.Put3DPnts(Terrain3D(diamond)),���ڵ��αȽ����⣬����Ҫ��ʱ�յ㣬ʹ�ã�World�ģ�����Ĳ����ࣨ3DSpaceR���ĺ�������������Ҳ�С�

//2.�߶������ʵ�
//��:�����غ� ��:����-λ��(�ٶȣ����ٶ�)
//Particle��Ӧ��Rule��ʼ��PhysicProp
//WorldR.PutWorldPnt("PhysicProp");

//3.��ʼ����
//������˵��һ�б��Ѵ��ڡ�����Դͷ���ǵĻ�̫���ӡ�
//��Ҫ��ͻȻ�����һ���ı����ء�
//WorldR.ChangeRule(golfPoint,...);

//4.ʱ���ݱ�
//��:�仯
//��:�ܱ�
//��仯����һ���ܱ䷶Χ������boundingBox����Ȼ���ܱ䷶Χ��������ܱ�

//5.����:������
//���ڼ�����洢�ռ����ޣ����Բ�������ϸ�ֿռ�ﵽ��������Ĺ���
//ֻ��ͨ��ָ�����㣬Ȼ����ѧ����surface��ײ
//���ԣ��ڵ��������У�SurfaceҲ��Ϊһ��ʵ�����

//##############################
//���¹���Ķ�
//1.����������̫����������涨��׺ΪY������ֱ�Ӳ��ӡ�
//2.����C++�������ƣ�3DSpace->Space3D,�Դ�����
//!!!
//3.�����������䱾�ʲ����ô�����������������ʽ����...��Ҫ�����߳��룬������ô������
//##############################
//Target List
//1.0 ����С��
//1.1 Yang-Space-MetricSpace-EuclideanSpace-Space3D
//1.2 Ying-Time
//1.3 Minkowski = Space3D + Time
//1.4 ��Space3DI����������ͽṹ���
//1.5 MinkowskiSpaceR����һ��FastParticle��ÿһ֡��world����evolve,��0֡��ʼ��10֡����
//1.6 Evolver-PhysicSolver-RigidSolver
//1.7 ��SetFrameSettingʱ��ʼ������Evolver����PutPntʱ��ע�⵽rule����physic��Ȼ���֮��֡������Evolver���PhysicSolver
//1.8 Log�������ݣ�txt�ļ�����Ȼ��UE4���/Houdini���Ŷ���
//1.9 MinkowskiSpaceR����2�������������桱��Ҳ����ͨ�õ������档
//2.0 ��PhysicSolver������ײ������ÿ��moveObj��
//2.0.1 ��ô�moveObj��old->prev��Ӱ�췶ΧeffectSpaceI
//2.0.2 ����effectSpaceI�����ཻ��effectSpaceO,����֮affect��ʵ�����Ƕ�Pnt�����ײ�󣬵�����λ�ú�����˲ʱ���ٶȣ�����
//2.1 ����˼·��ÿ��verlet�ı�Pntλ�õ�ʱ��Pnt��SetPosʱҲ������effectSpaceI����һ���߶�effectLine,�����´���effectTri��
//2.2 �����ṹ��Fast3DҪ����һ֡copy����Ϊÿ��Frame��Fast3D���Ƕ����ģ���̬collision����ָ�빲��
//2.3 bounce��verletʱ�洢v
//2.4 ����һ
//Yang-Data-GridData
//Ying-Arrangement-Arrange3D(3DSpaceY)-Terrain3D(����diamond displacement) 
//--WorldR.Put3DPnts(Terrain3D(diamond))
//WorldR.PutTri(Terrain3D(diamond).GetTri())
//2.5 ������
//FastGrid:����arr2<P>
//FastGrid grid(...);FastTerrainAlgo terrainAlgo(...);
//...
//grid.BindArrangeMent(terrainAlgo);
//WorldR.PutTri(grid.GetTri());
//2.6 ������
//Y:EuclideanSpace-Grid3D
//I:Grid3DI-FastGrid3D
//O:Grid3DO(����Arrangement)-TerrainAlgo-DiamondTerrainAlgo
//WorldR.PutTri(grid.GetTri());
//##############################

int main()
{
	MinkowskiSpace* world = new MinkowskiSpace;
	world->SetFrameSettings(361, 0.0166666);
	MinkowskiSpaceR* op = (MinkowskiSpaceR*)world->r[0];
	op->SetGravity(P(0.0, -9.80665, 0.0));

	//op->PutPnt("golf", P(1.5,4.0,1.5), "PhysicProp");
	op->PutPnt("golf", P(-2.8, 4.0, -2.8), "PhysicProp");

	//Tri tri1;
	//tri1.FromGrid(1.0, "xz", true);
	//tri1.Scale(100.0);
	//tri1.Transform(P(0, -4, 0));
	//!!! �������name��rule��û��
	//op->PutTri("grid_up", tri1, "CollisionProp");

	//Terrain
	Grid* terrain = new Grid;
	//terrain->SetGridSettings<double>(8, 1.0);
	terrain->SetGridSettings<double>(2, 3.0);
	GridR* op2 = (GridR*)terrain->r[0];
	op2->EasyTerrain(3.0,0.1);//terrain initH=3.0,roughness=0.1,detialLevel=MAX(100)
	//op2->SayI();
	op2->DebugSay();
	op2->DebugOutput("C:/HoudiniProjects/PToyScene/ttest.txt");
	arr<Tri> triArr;
	op2->TerrainToTri(triArr);
	op->PutTri("terrain", triArr, "CollisionProp");

	//op->PntInsForce(0, "golf", P(900.0, 400.0, 1000.0));
	op->Evolve(0);

	//p->SayI();
	//std::cout << "\n";
	//op->SayO();
	//std::cout << "\n";

	//op->Say(); //������֡�������
	//op->DebugSay();
	op->DebugOutput("C:/HoudiniProjects/PToyScene/dd.txt");

	int aa = 1;
}

// ���г���: Ctrl + F5 ����� >����ʼִ��(������)���˵�
// ���Գ���: F5 ����� >����ʼ���ԡ��˵�

// ������ʾ: 
//   1. ʹ�ý��������Դ�������������/�����ļ�
//   2. ʹ���Ŷ���Դ�������������ӵ�Դ�������
//   3. ʹ��������ڲ鿴���������������Ϣ
//   4. ʹ�ô����б��ڲ鿴����
//   5. ת������Ŀ��>���������Դ����µĴ����ļ�����ת������Ŀ��>�����������Խ����д����ļ���ӵ���Ŀ
//   6. ��������Ҫ�ٴδ򿪴���Ŀ����ת�����ļ���>���򿪡�>����Ŀ����ѡ�� .sln �ļ�

#endif