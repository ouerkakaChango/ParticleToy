// ParticleToy.cpp : ���ļ����� "main" ����������ִ�н��ڴ˴���ʼ��������
//
#ifdef THREEBODY
#include "pch.h"
#include <iostream>

#include "../include/Base/Spaces/Spaces.h"

//### �ܹ��ع���ǰհ
//1.Ϊʲô֡��֮֡���pnt����ָ�빲����Ҫ����?�����������ڴ棬������ɾ�����鷳
//��:��Ϊÿ֡���ݲ�һ���������ɸ��ƣ���Ȼ���ܾͱ��һ�������pnt��Ȼ����arr<Record>��ʵ�����塣
//��Ը�Թ������ݽ��ж���Ĵ�����Ż�����Ȼ��������ɾbug�ķ��գ��Գ���ԱҪ�󼫸ߡ�
//Ȩ��֮�»��ǲ������ַ������Ͼ���ʱ���ҷǱ�ʱ���ҡ�
//###
//### Ӧ��
//ParticleToy�ڶ���Ӧ�ã�������
//һ��˫�ǣ����£��켣
//1.ʹ��MinkowskiSpace������������򣨴�outer��Pnt,��ʱ�����ʵ��outerd����Sphere��
//�����յ��������µĴ����Է�����ײʱ���µ�����ֱ�Ӹ����գ��ٶ�Ҳ����
//2.����������G�ı�׼ֵ�����ٵ�ת����
//3.�ƶ�һ���淶��txt�ļ���ʽʹ�á���������������ײ�����ǵ���Ϣ�ڽ�������֡���û�ˣ�
//4.��������˫����ColliMerge(d)���ԣ���physicSolver��
//��colliMergeSolver������ǵ�����ײ��֮���Ե����������������collisionSolver�
//����Ϊ����һ����merge�ģ��ʹ�ͳ��ײSolver��һ��

int main()
{
	MinkowskiSpace* world = new MinkowskiSpace;
	world->SetFrameSettings(1000, 0.0166666);
	MinkowskiSpaceR* op = (MinkowskiSpaceR*)world->r[0];

	Pnt sun(P(0, 0, 0));
	sun.name = "sun";
	sun.rule = "Space ColliMerged";
	sun.mass = 10.0;
	sun.v = P(0.0, 0.0, 0.0);
	sun.SetSphereOuter(0.1);
	op->PutPnt(sun);

	Pnt moon(P(0, 0, -1));
	moon.name = "moon";
	moon.rule = "Space ColliMerge";
	moon.mass = 10;
	moon.v = P(1, 0, 1)*3.2;
	moon.SetSphereOuter(0.1);
	op->PutPnt(moon);

	Pnt moon2(P(0, 0, 1));
	moon2.name = "moon2";
	moon2.rule = "Space ColliMerge";
	moon2.mass = 10;
	moon2.v = P(-1, 0, -1)*3.2;
	moon2.SetSphereOuter(0.1);
	//op->MoveToFrame(40);
	op->PutPnt(moon2);

	op->Evolve(0);

	op->OutputPntTrajTxt("C:/HoudiniProjects/PToyScene/sunMoonTraj.txt");
	return 0;
}
#endif