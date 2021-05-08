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

	SolvePntWithPnt(prev, newPnt, dt, info);
	if (newPnt.ignorePrev)
	{
		ForceSolvePntWithTri(newPnt);
		newPnt.ignorePrev = false;
	}
	else if (newPnt.isBreakPnt)
	{
		auto vPrev = GetVirtualOldPnt(newPnt, dt);
		SolvePntWithTri(vPrev, newPnt, dt, info);
	}
	else
	{
		SolvePntWithTri(prev, newPnt, dt, info);
	}
	
}

void CollisionSolver::ForceSolvePntWithTri(Pnt& pnt)
{
	for (int inx = 0; inx < triArr->size(); inx++)
	{
		auto& tri = (*triArr)[inx];
		if (tri.IsPointUnder(pnt.pos, pnt.outer))
		{
			pnt.pos = tri.GetFixedPos(pnt.pos, pnt.outer);
			pnt.UpdateEffectSpace();
		}
	}
}

void CollisionSolver::SolvePntWithTri(const Pnt& prev, Pnt& newPnt, double dt, ExtraInfo info)
{
	//Solve��켣��tri����ײ
	for (int inx = 0; inx < triArr->size(); inx++)
	{
		auto& tri = (*triArr)[inx];
		auto interInfo = newPnt.effectSpace.Collide(tri);
		if (interInfo.result)
		{
			P hitP = interInfo.hitP;
			if (equal(hitP, prev.pos) && tri.IsPointUnder(newPnt.pos, newPnt.outer))
			{
				//!!! ���������prev��ʵ�Ѿ����������棬��ôֱ�ӽ�newPntǿ���Ƶ����������ϡ�
				//�������ڶ�����֧�������㣬����cap��tri��hitP�᷵��S1��λ�ã�����newPnt������
				newPnt.pos = tri.GetFixedPos(newPnt.pos, newPnt.outer);
				newPnt.UpdateEffectSpace();
				P vPartAlignNorm = tri.n * dot(tri.n, newPnt.v);
				newPnt.v -= vPartAlignNorm;
				//!!! �����return,���ٽ�����ײ�жϳ�����
				return;
			}
			else if (!tri.IsPointFixed(newPnt.pos, newPnt.outer))
			{
				//1-2.
				double dtc = -1.0;//delta time until collision
				double x1 = 0.0, x2 = 0.0;
				bool bSolve = SolveQuadra(prev.a / 2, prev.v, prev.pos - hitP, x1, x2);
				if (!bSolve)
				{
					//???
					abort();
					return;
				}
				else if ((x1<0||x1>dt)&&(x2<0||x2>dt))
				{
					//����ĳЩԭ��?�����²������ȼ����˶�ģ��dtʱ����prev��new����Ϊ��
					//(��������һ֡���ļ��ٶ����dt�����ˣ���prev.aû��������)
					//��ôֱ���ϼ����ٶ�damp��
					//Ҳ������һ֡��PBC����ô������ʵ��
					//!!!
					//PS:��ʵ�Ǳ�����˶��������Ҫ�������ȱ�����˶����ƣ������Ȳ�����
					newPnt.pos = tri.GetFixedPos(newPnt.pos, newPnt.outer);
					newPnt.UpdateEffectSpace();
					newPnt.v = reflect(newPnt.v, tri.n) * bounceDamp;
					{
						Pnt breakPnt(newPnt.pos);
						breakPnt.a = newPnt.a;
						breakPnt.v = newPnt.v;
						newPnt.SetBreakPoint(breakPnt, dt);
					}
					return;
				}
				else
				{
					dtc = QuadraFiliter(x1, x2, 0, dt);
					//3.
					P v2 = prev.v + prev.a*dtc;
					P v3 = reflect(v2, tri.n)*bounceDamp;
					//4.
					double dtr = dt - dtc;
					newPnt.pos = hitP + (v3*dtr + 0.5*newPnt.a*dtr*dtr);
					newPnt.UpdateEffectSpace();
					newPnt.v = v3 + newPnt.a*dtr;
					//!!!
					//������˵�������ĵ㲻�������ײ�棻
					//������verlert�����ԭ�򣨱������º�����ͣ�µ�С�򣩣�����v3��(?)������ȷ��Ȼ��λ�ò���
					if (tri.IsPointUnder(newPnt.pos, newPnt.outer))
					{
						newPnt.pos = hitP;
						newPnt.UpdateEffectSpace();
						P vPartAlignNorm = tri.n * dot(tri.n, newPnt.v);
						newPnt.v -= vPartAlignNorm;
					}
					//___
					//5.Setup breakPnt
					{
						Pnt breakPnt(hitP);
						breakPnt.a = prev.a;
						breakPnt.v = v3;
						newPnt.SetBreakPoint(breakPnt, dtr);
					}
					return;
				}
			}
		}
	}
}

void CollisionSolver::SolvePntWithPnt(const Pnt& prev, Pnt& newPnt, double dt, ExtraInfo info)
{
	for (int inx = 0; inx < pntArr.size(); inx++)
	{
		int p2Inx = pntArr[inx];
		if (info.pntInx != p2Inx)
		{
			auto& other = (*info.newPnts)[p2Inx];
			//�������߶��ڶ���������outer���ж��ཻ��������effectSpace
			auto interInfo = newPnt.Collide(other);
			//???
			bool go = true;
			//if ( equal(interInfo.d, 0, 1.0e-4))
			//{
			//	P n1 = norm(newPnt.pos - other.pos);
			//	newPnt.pos = other.pos + (interInfo.r1 + interInfo.r2)*n1;
			//	newPnt.v = reflect(newPnt.v, n1);
			//	P vPartAlignNorm = n1 * dot(n1, newPnt.v);
			//	newPnt.v -= vPartAlignNorm;
			//	newPnt.ignorePrev = true;
			//	go = false;
			//}		
			//___
			if (go&&interInfo.result)
			{
				auto& p1 = (*info.prevPnts)[info.pntInx];
				auto& p2 = (*info.prevPnts)[p2Inx];
				double r1 = interInfo.r1;
				double r2 = interInfo.r2;
				//�����ཻ��������ShapeӦ��Ҳ��һ�����Ժ���֤��ȷ������������Pnt��Tri��ײ������PBC����
				auto func = [=](double t)
				{
					P A = 0.5*(p2.a - p1.a);
					P B = p2.v - p1.v;
					P C = p2.pos - p1.pos;
					P L = A * t*t + B * t + C;
					return L.len() - r1 - r2;
				};
				double dtc = -1.0;
				bool bSolve = BisecitonSolve(func, 0.0, dt, dtc);
				if (!bSolve)
				{
					//����ֹ�����
					if (func(0) < 0 && func(dt < 0))
					{
						P n1 = norm(newPnt.pos - other.pos);
						//!!! ����ֹ��������λ�ò���
						newPnt.pos = other.pos + (interInfo.r1 + interInfo.r2)*n1;
						newPnt.v = reflect(newPnt.v, n1);
						P vPartAlignNorm = n1 * dot(n1, newPnt.v);
						newPnt.v -= vPartAlignNorm;
						newPnt.ignorePrev = true;
					}
					
				}
				else
				{
					//�׶�1������һ��0-dtc
					//�׶�2:�ٶȷ�����damp������dtʱ��λ��
					//calcu collision plane
					double dtr = dt - dtc;
					P n1 = norm(newPnt.pos - other.pos);

					newPnt.v = p1.v + p1.a * dtc;
					newPnt.v = reflect(newPnt.v, n1)*bounceDamp;
					P hitP1 = p1.pos + p1.v*dtc + 0.5*p1.a*dtc*dtc;
					newPnt.pos = hitP1 + newPnt.v*dtr + 0.5*newPnt.a*dtr*dtr;
					newPnt.UpdateEffectSpace();

					//other.v = p2.v + p2.a * dtc;
					//other.v = reflect(other.v, -n1)*bounceDamp;
					//P hitP2 = p2.pos + p2.v*dtc + 0.5*p2.a*dtc*dtc;
					//other.pos = hitP2 + other.v*dtr + 0.5*other.a*dtr*dtr;
					//other.UpdateEffectSpace();

					//breakPnt for p1,p2
					{
						Pnt breakPnt(hitP1);
						breakPnt.a = p1.a;
						breakPnt.v = newPnt.v;
						newPnt.SetBreakPoint(breakPnt, dtr);
						lastPntColliVirtalPont = true;
					}
					{
						//Pnt breakPnt(hitP2);
						//breakPnt.a = p2.a;
						//breakPnt.v = other.v;
						//other.SetBreakPoint(breakPnt, dtr);
					}
				}
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