#include "FastPnt.h"

//### Pnt
Pnt::Pnt()
{

}

Pnt::Pnt(P pos_) :pos(pos_)
{

}

Pnt::Pnt(P pos_, str name_, str rule_) : 
	pos(pos_), name(name_), rule(rule_)
{

}

void Pnt::SetSphereOuter(double r)
{
	if (outer != nullptr)
	{
		abort();
	}
	outer = new Sphere(r);
}

void Pnt::UpdateOuter() const
{
	if (outer == nullptr)
	{
		abort();
	}
	if (typeid(outer).name() == "class Sphere")
	{
		auto s = static_cast<Sphere*>(outer);
		s->center = pos;
	}
}

bool Pnt::Collide(const Pnt& pnt)
{
	UpdateOuter();
	pnt.UpdateOuter();

	if (outer == nullptr || pnt.outer == nullptr)
	{
		abort();
	}
	else
	{
		return outer->Collide(pnt.outer);
	}
}

str Pnt::InfoString(int precision)
{
	str re = "";
	re.AddDouble(pos.x, precision);
	re += " ";
	re.AddDouble(pos.y, precision);
	re += " ";
	re.AddDouble(pos.z, precision);

	re += "    ";
	re.AddDouble(v.x, precision);
	re += " ";
	re.AddDouble(v.y, precision);
	re += " ";
	re.AddDouble(v.z, precision);
	return re;
}

str Pnt::InfoString(str filter, int precision)
{
	str re;
	if (filter == "pos")
	{
		re.AddDouble(pos.x, precision);
		re += " ";
		re.AddDouble(pos.y, precision);
		re += " ";
		re.AddDouble(pos.z, precision);
	}

	return re;
}

str Pnt::TxtHeadString()
{
	str re;
	re += name;
	re += " ";
	if (outer != nullptr)
	{
		re += outer->TxtHeadString();
	}
	return re;
}

bool Pnt::IsBreakPoint() const
{
	return isBreakPnt;
}

void Pnt::SetBreakPoint(const Pnt& pnt, double dtr_)
{
	dtr = dtr_;
	if (dtr < 0)
	{
		abort();
	}
	effectSpace.ResetPrev(pnt.pos);
	breakPnt = new Pnt;
	(*breakPnt) = pnt;
	isBreakPnt = true;
}

Pnt Pnt::GetBreakPnt() const
{
	return *breakPnt;
}

Pnt Pnt::GetVirtualOldPnt(double dt) const
{
	if (!isBreakPnt)
	{
		abort();
	}
	Pnt re = *this;
	re.v -= a * dt;
	//dr = v0*dt+0.5*a*dt^2
	re.pos -= re.v*dt + 0.5 * a *dt*dt;
	return re;
}
//### Pnt