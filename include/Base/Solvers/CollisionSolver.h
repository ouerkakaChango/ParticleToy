#pragma once

#include "SolverUtility.h"

//�������壺��Ϊ����ԭ�����Բ���physicSolver�ﾫȷ��֧���������ü��εİ취
//ǿ��λ�ã���ʹ��һ��������ʽ��������"Physic Based Collision"������Physic

class CollisionSolver
{
public:
	void Load(const arr<Tri>* triArr_);
	void Solve(const Pnt& prev, Pnt& newPnt, double dt);
	Pnt GetVirtualOldPnt(const Pnt& prev, double dt) const;
	const Tri& GetTri(int uid) const;

	const arr<Tri>* triArr;
	double bounceDamp = 0.8;
};