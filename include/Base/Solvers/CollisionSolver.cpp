#include "CollisionSolver.h"

//### CollisionSolver
void CollisionSolver::Load(const arr<Tri>* triArr_)
{
	triArr = triArr_;
}

void CollisionSolver::Solve(const Pnt& prev, Pnt& newPnt, double dt, ExtraInfo info)
{
	//�Դ�PBC��Physical Based Collision���㷨�� ���ڼ�����ʽ���㣬������Ӳ����ײ���ֱ���ÿ���
	//��ϸ�����Ƶ��ڣ�https://blog.csdn.net/qq_41524721

	//Solve��켣��tri����ײ
	for (int inx = 0; inx < triArr->size(); inx++)
	{
		auto& tri = (*triArr)[inx];
		auto interInfo = newPnt.effectSpace.Collide(tri);
		if (interInfo.result)
		{
			P hitP = interInfo.hitP;
			if (equal(hitP, prev.pos)&& tri.IsPointUnder(newPnt.pos, newPnt.outer))
			{
				//!!! ���������prev��ʵ�Ѿ����������棬��ôֱ�ӽ�newPntǿ���Ƶ����������ϡ�
				//�������ڶ�����֧�������㣬����cap��tri��hitP�᷵��S1��λ�ã�����newPnt������
				newPnt.pos = tri.GetFixedPos(newPnt.pos,newPnt.outer);
				newPnt.effectSpace.Update(newPnt);
				P vPartAlignNorm = tri.n * dot(tri.n, newPnt.v);
				newPnt.v -= vPartAlignNorm;
			}
			else if(!tri.IsPointFixed(newPnt.pos, newPnt.outer))
			{
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
				newPnt.effectSpace.Update(newPnt);
				newPnt.v = v3 + newPnt.a*dtr;
				//!!!
				//������˵�������ĵ㲻�������ײ�棻
				//������verlert�����ԭ�򣨱������º�����ͣ�µ�С�򣩣�����v3��(?)������ȷ��Ȼ��λ�ò���
				if (tri.IsPointUnder(newPnt.pos, newPnt.outer))
				{
					newPnt.pos = hitP;
					newPnt.effectSpace.Update(newPnt);
					P vPartAlignNorm = tri.n * dot(tri.n, newPnt.v);
					newPnt.v -= vPartAlignNorm;
				}
				//___
				//5.Setup breakPnt
				{
					Pnt breakPnt(hitP);
					breakPnt.a = prev.a;
					breakPnt.v = v3;
					newPnt.SetBreakPoint(tri.uid, breakPnt, dtr);
				}
			}
		}
	}

	for (int inx = 0; inx < pntArr.size(); inx++)
	{
		int p2Inx = pntArr[inx];
		if (info.pntInx != p2Inx)
		{
			auto& other = (*info.newPnts)[p2Inx];
			//�������߶��ڶ���������outer���ж��ཻ��������effectSpace
			auto interInfo = newPnt.Collide(other);
			if (interInfo.result)
			{
				auto& p1 = (*info.prevPnts)[info.pntInx];
				auto& p2 = (*info.prevPnts)[p2Inx];
				double r1 = interInfo.r1;
				double r2 = interInfo.r2;
				//�����ཻ��������ShapeӦ��Ҳ��һ�����Ժ���֤��ȷ������������Pnt��Tri��ײ������PBC����
				auto func = [=](double t)
				{
					P A = 0.5*(p2.a-p1.a);
					P B = p2.v - p1.v;
					P C = p2.pos - p1.pos;
					P L = A * t*t + B * t + C;
					return L.len() - r1 - r2;
				};
				double dtc = -1.0;
				BisecitonSolve(func,0.0,dt, dtc);
				//??? debug
				auto func2 = [=](double t)
				{
					return t * t - 3;
				};
				bool bb = BisecitonSolve(func2, 0.0, 2, dtc);
				//___
				int aa = 1;
			}
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
	return re;
}

void CollisionSolver::Clear()
{
	pntArr.clear();
	triArr = nullptr;
}
//### CollisionSolver