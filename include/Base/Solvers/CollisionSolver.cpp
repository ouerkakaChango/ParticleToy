#include "CollisionSolver.h"

//### CollisionSolver
void CollisionSolver::Load(const arr<Tri>* triArr_)
{
	triArr = triArr_;
}

void CollisionSolver::Solve(const Pnt& prev, Pnt& newPnt, double dt, ExtraInfo info)
{
	//自创PBC（Physical Based Collision）算法。 基于简单物理公式解算，而不是硬将碰撞物件直接拿开。
	//详细物理推导在：https://blog.csdn.net/qq_41524721

	//Solve点轨迹与tri的碰撞
	for (int inx = 0; inx < triArr->size(); inx++)
	{
		auto& tri = (*triArr)[inx];
		auto interInfo = newPnt.effectSpace.Collide(tri);
		if (interInfo.result)
		{
			P hitP = interInfo.hitP;
			if (equal(hitP, prev.pos)&& tri.IsPointUnder(newPnt.pos, newPnt.outer))
			{
				//!!! 极限情况，prev其实已经紧贴三角面，那么直接将newPnt强制移到三角面以上。
				//如果进入第二个分支，会多计算，而且cap和tri的hitP会返回S1的位置，导致newPnt动不了
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
				{//好像不可能存在这种情况
					abort();
				}
				//选择正值
				dtc = x1;
				if (x1 < 0)
				{
					dtc = x2;
				}
				if (dtc >= dt)
				{//好像不可能存在这种情况
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
				//照理来说，解算后的点不会低于碰撞面；
				//但由于verlert的误差原因（比如落下后慢慢停下的小球），导致v3项(?)不够精确，然后位置不对
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
			//由于两者都在动，所以用outer来判断相交，而不是effectSpace
			auto interInfo = newPnt.Collide(other);
			if (interInfo.result)
			{
				auto& p1 = (*info.prevPnts)[info.pntInx];
				auto& p2 = (*info.prevPnts)[p2Inx];
				double r1 = interInfo.r1;
				double r2 = interInfo.r2;
				//对于相交两球（其他Shape应该也是一样，以后验证正确），类似上面Pnt与Tri碰撞，进行PBC解算
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