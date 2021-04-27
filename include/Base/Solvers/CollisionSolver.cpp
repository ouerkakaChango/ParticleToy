#include "CollisionSolver.h"

//### CollisionSolver
void CollisionSolver::Load(const arr<Tri>* triArr_)
{
	triArr = triArr_;
}

void CollisionSolver::Solve(const Pnt& prev, Pnt& newPnt, double dt)
{
	//Solve��켣�����߶���tri����ײ
	for (int inx = 0; inx < triArr->size(); inx++)
	{
		auto& tri = (*triArr)[inx];
		auto interInfo = newPnt.effectSpace.Collide(tri);
		if (interInfo.result)
		{
			auto hitP = interInfo.hitP;
			//1-2.
			double dtc = -1.0;//delta time until collision
			double x1 = 0.0, x2 = 0.0;
			SolveQuadra(prev.a / 2, prev.v, prev.pos - hitP, x1, x2);
			if (x1 * x2 > 0)
			{//���񲻿��ܴ����������
				abort();
			}
			//ѡ����ֵ
			dtc = x1;
			if (x1 < 0)
			{
				dtc = x2;
			}
			if (dtc >= dt)
			{//���񲻿��ܴ����������
				abort();
			}
			//3.
			P v2 = prev.v + prev.a*dtc;
			P v3 = reflect(v2, tri.n)*bounceDamp;
			//4.
			double dtr = dt - dtc;
			newPnt.pos = hitP + (v3*dtr + 0.5*newPnt.a*dtr*dtr);
			newPnt.v = v3 + newPnt.a*dtr;
			//!!!
			//������˵�������ĵ㲻�������ײ�棻
			//������verlert�����ԭ�򣨱������º�����ͣ�µ�С�򣩣�����v3��(?)������ȷ��Ȼ��λ�ò���
			if (tri.IsPointUnder(newPnt.pos, newPnt.outer))
			{
				newPnt.pos = hitP;
				newPnt.v = P(0, 0, 0);
			}
			//___
			newPnt.effectSpace.Update(newPnt);
			//5.Setup breakPnt
			Pnt breakPnt(hitP);
			breakPnt.a = prev.a;
			breakPnt.v = v3;
			newPnt.SetBreakPoint(tri.uid, breakPnt, dtr);
		}
	}
}

const Tri& CollisionSolver::GetTri(int uid) const
{
	for (int inx = 0; inx < triArr->size(); inx++)
	{
		auto& tri = (*triArr)[inx];
		if (tri.uid == uid)
		{
			return tri;
		}
	}
	abort();
	return (*triArr)[0];
}

Pnt CollisionSolver::GetVirtualOldPnt(const Pnt& prev, double dt) const
{
	if (!prev.isBreakPnt)
	{
		abort();
	}
	Pnt re = prev;
	re.v -= prev.a * dt;
	re.pos -= re.v*dt + 0.5 * prev.a *dt*dt;
	auto& tri = GetTri(prev.uid_breakPlane);
	//???
	double dp = (re.pos - prev.pos).len();
	//??? �ĳ�ͨ��dt����׼ȷ��dp���ޣ�������0.001
	if (dp < 0.001)
	{
		auto aa = 1;
	}
	//___
	return re;
}
//### CollisionSolver