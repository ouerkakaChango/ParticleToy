#include "CollisionSolver.h"

//### CollisionSolver
void CollisionSolver::Load(const arr<Tri>* triArr_)
{
	triArr = triArr_;
}

void CollisionSolver::Solve(const Pnt& prev, Pnt& newPnt, double dt)
{
	//Solve点轨迹：射线段与tri的碰撞
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
			newPnt.v = v3 + newPnt.a*dtr;
			newPnt.effectSpace.Update(newPnt);
			//5.Setup breakPnt
			Pnt breakPnt(hitP);
			breakPnt.a = prev.a;
			breakPnt.v = v3;
			newPnt.SetBreakPoint(breakPnt, dtr);
		}
	}
}
//### CollisionSolver