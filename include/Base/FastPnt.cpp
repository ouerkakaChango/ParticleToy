#include "FastPnt.h"

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
	str re = "";
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