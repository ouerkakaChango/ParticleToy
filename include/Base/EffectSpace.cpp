#include "EffectSpace.h"
#include "FastPnt.h"

//### EffectSpace
EffectSpace::EffectSpace()
{
}

void EffectSpace::DefineLineI(P a_, P b_)
{
	type = "LineI";
	auto newI = new EffectLineI;
	newI->Set(a_, b_);
	i += newI;
	isDefined = true;
}

void EffectSpace::DefineCapsuleI(P a, P b, double r)
{
	type = "CapsuleI";
	auto newI = new EffectCapsuleI;
	newI->Set(a, b, r);
	i += newI;
	isDefined = true;
}

IntersectInfo EffectSpace::Collide(const Tri& tri)
{
	IntersectInfo re;
	if (isDefined && !isIgnore)
	{
		auto ti = Cast<EffectSpaceI*>(i[0]);
		re = ti->Collide(tri);
	}
	return re;
}

void EffectSpace::Update(const Pnt& pnt)
{
	if (!isIgnore)
	{
		if (type == "LineI")
		{
			auto line = Cast<EffectLineI*>(i[0]);
			line->UpdateB(pnt.pos);
		}
		else if (type == "CapsuleI")
		{
			auto cap = Cast<EffectCapsuleI*>(i[0]);
			cap->UpdateB(pnt.pos);
		}
	}
}

void EffectSpace::SafeUpdate(const Pnt& prevPnt, Pnt& pnt)
{
	if (!isDefined)
	{
		if (prevPnt.outer == nullptr && pnt.outer == nullptr)
		{//两者outer都是空，直线
			DefineLineI(prevPnt.pos, pnt.pos);
		}
		else if (prevPnt.outer == nullptr || pnt.outer == nullptr)
		{//一者outer非空一者空，abort
			abort();
		}
		else if (typeStr(*prevPnt.outer) != typeid(*pnt.outer).name())
		{//!!! 两者outer形状不一样
			abort();
		}
		else
		{
			auto aa = typeStr(*prevPnt.outer);
			if (typeStr(*prevPnt.outer) == "class Sphere")
			{
				auto sphere1 = static_cast<Sphere*>(prevPnt.outer);
				auto sphere2 = static_cast<Sphere*>(pnt.outer);
				if (!equal(sphere1->r, sphere2->r))
				{//!!! 两球半径不一
					abort();
				}
				else
				{//两球半径一致，定义胶囊
					DefineCapsuleI(prevPnt.pos, pnt.pos, sphere1->r);
				}
			}
		}
	}
	else
	{
		Update(pnt);
	}
}

void EffectSpace::ResetPrev(P p)
{
	if (!isIgnore)
	{
		if (type == "LineI")
		{
			auto line = Cast<EffectLineI*>(i[0]);
			line->l.a = p;
		}
		else if (type == "CapsuleI")
		{
			auto cap = Cast<EffectCapsuleI*>(i[0]);
			cap->capsule.a = p;
		}
	}
}
//### EffectSpace

//### EffectSpaceI
IntersectInfo EffectSpaceI::Collide(const Tri& tri)
{
	IntersectInfo re;
	return re;
}
//### EffectSpaceI

//### EffectLineI
void EffectLineI::Set(P a, P b)
{
	l.Set(a, b);
}

void EffectLineI::UpdateB(P newb)
{
	l.UpdateB(newb);
}

IntersectInfo EffectLineI::Collide(const Tri& tri)
{
	IntersectInfo re;
	re = tri.Collide(l);
	return re;
}
//### EffectLineI

//### EffectCapsuleI
void EffectCapsuleI::Set(P a, P b, double r)
{
	capsule.Set(a, b, r);
}

void EffectCapsuleI::UpdateB(P newb)
{
	capsule.UpdateB(newb);
}

IntersectInfo EffectCapsuleI::Collide(const Tri& tri)
{
	IntersectInfo re;
	re = tri.Collide(capsule);
	return re;
}
//### EffectCapsuleI