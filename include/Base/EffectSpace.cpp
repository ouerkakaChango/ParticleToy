#include "EffectSpace.h"

//### EffectSpace
EffectSpace::EffectSpace()
{
}

void EffectSpace::Define(str type)
{
	if (type == "lineI")
	{
		auto newI = new EffectLineI;
		i += newI;
	}
	isDefined = true;
}

IntersectInfo EffectSpace::Intersect(const Tri& tri)
{
	IntersectInfo re;
	if (isDefined)
	{
		auto ti = Cast<EffectSpaceI*>(i[0]);
		re = ti->Intersect(tri);
	}
	return re;
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