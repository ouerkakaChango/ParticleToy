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
			line->Update(p);
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
	line.Set(a, b);
}

void EffectLineI::Update(P newb)
{
	line.Update(newb);
}

IntersectInfo EffectLineI::Intersect(const Tri& tri)
{
	IntersectInfo re;
	re = tri.Intersect(line);
	return re;
}
//### EffectLineI