#include "FastGeo.h"

#include "EffectSpace.h"
//### Pnt
Pnt::Pnt()
{

}

Pnt::Pnt(P pos_) :pos(pos_)
{

}

Pnt::Pnt(P pos_, str rule_) : pos(pos_), rule(rule_)
{

}

str Pnt::InfoString(int precision)
{
	str re = "";
	re.AddDouble(pos.x, precision);
	re += " ";
	re.AddDouble(pos.y, precision);
	re += " ";
	re.AddDouble(pos.z, precision);
	return re;
}

void Pnt::EffectUpdate(const Pnt& prev)
{
	if (effectSpace == nullptr)
	{
		effectSpace = new EffectSpace;
	}
	if (!effectSpace->IsDefined())
	{
		effectSpace->Define("lineI");
		auto line = Cast<EffectLineI*>(effectSpace->i[0]);
		line->Set(prev.pos, pos);
	}
	else
	{
		auto line = Cast<EffectLineI*>(effectSpace->i[0]);
		line->Update(pos);
	}
}
//### Pnt

//### Tri
void Tri::FromGrid(double len, str filter, bool isUpTri)
{
	if (isUpTri)
	{
		p1 = len * P(P2(-1, -1), filter);
		p2 = len * P(P2(-1, 1), filter);
		p3 = len * P(P2(1, 1), filter);
	}
	else
	{
		p1 = len * P(P2(1, 1), filter);
		p2 = len * P(P2(1, -1), filter);
		p3 = len * P(P2(-1, -1), filter);
	}
}

void Tri::Transform(P offset)
{
	p1 += offset;
	p2 += offset;
	p3 += offset;
}
//### Tri