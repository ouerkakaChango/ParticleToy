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

//### Line
void Line::Set(P a_, P b_)
{
	a = a_;
	b = b_;
}

void Line::Update(P newb)
{
	a = b;
	b = newb;
}

P Line::dir() const
{
	return norm(b - a);
}
//### Line

//### Global Line
double len(const Line& l)
{
	return len(l.b - l.a);
}
//### Global Line

//### Plane
void Plane::Define(P n_, P p_)
{
	n = n_;
	p = p_;
}

void Plane::Transform(P offset)
{
	p += offset;
}

IntersectInfo Plane::Intersect(const Line& l) const
{
	IntersectInfo re;
	//原理：https://blog.csdn.net/qq_41524721/article/details/103490144
	P T = l.a - p;
	double d = -dot(l.a - p, n) / dot(l.dir(), n);
	if (d <= len(l))
	{
		re.hit = true;
		re.d = d;
		re.hitP = l.a + d * l.dir();
	}
	return re;
}
//### Plane

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
	CalculateNormal();
	Plane::Define(n, p1);
}

void Tri::Transform(P offset)
{
	Plane::Transform(offset);
	p1 += offset;
	p2 += offset;
	p3 += offset;
}

void Tri::CalculateNormal()
{
	//!!!Houdini Coordinate
	//p2p3 cross p2p1
	n = cross((p3 - p2),(p1 - p2));
	n = norm(n);
}

bool Tri::IsPointInside(P p) const
{
	P v0 = p3 - p1;
	P v1 = p2 - p1;
	P v2 = p - p1;

	double dot00 = dot(v0, v0);
	double dot01 = dot(v0, v1);
	double dot02 = dot(v0, v2);
	double dot11 = dot(v1, v1);
	double dot12 = dot(v1, v2);

	double invDeno = 1 / (dot00*dot11 - dot01 * dot01);
	
	double u = (dot11*dot02 - dot01 * dot12)*invDeno;
	if (u < 0 || u>1)
	{
		return false;
	}

	double v = (dot00*dot12 - dot01 * dot02)*invDeno;
	if (v < 0 || v>1)
	{
		return false;
	}

	return u + v <= 1;
}

IntersectInfo Tri::Intersect(const Line& l) const
{
	IntersectInfo re;
	//1.判断与平面相交
	re = Plane::Intersect(l);
	//2.判断交点是否在三角形内
	if (re.hit)
	{
		P hitP = l.a + re.d*l.dir();
		//所谓"重心法"：https://blog.csdn.net/wkl115211/article/details/80215421
		re.hit = IsPointInside(hitP);
	}
	return re;
}
//### Tri