#include "Fast3D.h"

//### P2
P2::P2(int x_, int y_):x(x_),y(y_)
{

}
//### P2

//### P
P::P()
{

}

P::P(double x_, double y_, double z_):x(x_),y(y_),z(z_)
{

}

P::P(int x_, int y_, int z_) : 
	x(static_cast<double>(x_)), 
	y(static_cast<double>(y_)), 
	z(static_cast<double>(z_))
{
}

P::P(P2 p2, str filter)
{
	if (filter == "xz")
	{
		x = p2.x;
		z = p2.y;
	}
}

P P::operator*(double s) const
{
	return P(x*s, y*s, z*s);
}

P P::operator*(P p) const
{
	return P(x*p.x, y*p.y, z*p.z);
}

P operator*(double s, const P& p)
{
	return p * s;
}

P P::operator+(const P& p)
{
	return P(x + p.x, y + p.y, z + p.z);
}

P P::operator-(const P& p)
{
	return P(x - p.x, y - p.y, z - p.z);
}

void P::operator+=(const P& p)
{
	x += p.x;
	y += p.y;
	z += p.z;
}
//### P

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
//### Tri

Pnt::Pnt()
{

}

Pnt::Pnt(P pos_):pos(pos_)
{

}

Pnt::Pnt(P pos_, str rule_) : pos(pos_),rule(rule_)
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

void Fast3D::AddPnt(str name, P pos, str rule)
{
	pnts += Pnt(pos,rule);
}

str Fast3D::InfoString(int precision)
{
	str re = "";
	for (auto& p : pnts)
	{
		re += p.InfoString(precision);
	}
	return re;
}