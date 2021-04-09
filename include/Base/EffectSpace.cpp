#include "EffectSpace.h"

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

IntersectInfo EffectSpace::Intersect(const Tri& tri)
{
	IntersectInfo re;
	if (isDefined && !isIgnore)
	{
		auto ti = Cast<EffectSpaceI*>(i[0]);
		re = ti->Intersect(tri);
	}
	return re;
}

void EffectSpace::Update(P p)
{
	if (!isIgnore)
	{
		if (type == "LineI")
		{
			auto line = Cast<EffectLineI*>(i[0]);
			line->UpdateB(p);
		}
	}
}

void EffectSpace::SafeUpdate(P prevP, P p)
{
	if (!isDefined)
	{
		DefineLineI(prevP, p);
	}
	else
	{
		Update(p);
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
	}
}
//### EffectSpace

//### EffectSpaceI
IntersectInfo EffectSpaceI::Intersect(const Tri& tri)
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

void EffectLineI::UpdateA(P newb)
{

}

void EffectLineI::UpdateB(P newb)
{
	l.UpdateB(newb);
}

IntersectInfo EffectLineI::Intersect(const Tri& tri)
{
	IntersectInfo re;
	re = tri.Intersect(l);
	return re;
}
//### EffectLineI