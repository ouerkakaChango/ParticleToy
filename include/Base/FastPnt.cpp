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

void Pnt::UpdateEffectSpace()
{
	effectSpace.Update(*this);
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
	if (typeStr(*outer) == "class Sphere")
	{
		auto s = static_cast<Sphere*>(outer);
		s->center = pos;
	}
}

IntersectInfo Pnt::Collide(const Pnt& pnt)
{
	UpdateOuter();
	pnt.UpdateOuter();

	if (outer == nullptr || pnt.outer == nullptr)
	{
		abort();
	}
	else
	{
		return outer->Collide(*pnt.outer);
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
	if (filter.Has("pos"))
	{
		re.AddDouble(pos.x, precision);
		re += " ";
		re.AddDouble(pos.y, precision);
		re += " ";
		re.AddDouble(pos.z, precision);
		re += " ";
	}
	if (filter.Has("v"))
	{
		re.AddDouble(v.x, precision);
		re += " ";
		re.AddDouble(v.y, precision);
		re += " ";
		re.AddDouble(v.z, precision);
		re += " ";
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
//### Pnt

//### Global Pnt
bool RuleOf(str rule, str prop)
{
	if (prop == "Collision")
	{
		if (rule.Has("Collision"))
		{
			return true;
		}
		if (rule.Has("PhysicProp"))
		{
			return true;
		}
		if (rule.Has("Molecule"))
		{
			return true;
		}
	}
	else if (prop == "PhysicProp")
	{
		if (rule.Has("PhysicProp"))
		{
			return true;
		}
		if (rule.Has("Space"))
		{
			return true;
		}
		if (rule.Has("Molecule"))
		{
			return true;
		}
	}
	else if (prop == "UniversalG")
	{
		if (rule.Has("Space"))
		{
			return true;
		}
		//if (rule.Has("Molecule"))
		//{
		//	return true;
		//}
	}
	else if (prop == "RelationForce")
	{
		if (rule.Has("Molecule"))
		{
			return true;
		}
	}
	return false;
}
//### Global Pnt