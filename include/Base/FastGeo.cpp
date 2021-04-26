#include "FastGeo.h"

//### Shape
bool Shape::Collide(const Shape* other)
{
	if (other == nullptr)
	{
		abort();
	}
	return false;
}

bool Shape::IsPointInside(P p) const
{
	return false;
}

str Shape::TxtHeadString()
{
	str re;
	return re;
}
//### Shape

//### Line
void Line::Set(P a_, P b_)
{
	a = a_;
	b = b_;
}

void Line::UpdateB(P newb)
{
	if (equal(b.y, newb.y))
	{
		int aa = 1;
	}
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

//### Capsule
void Capsule::Set(P a_, P b_, double r_)
{
	a = a_;
	b = b_;
	r = r_;
}

Sphere Capsule::GetS1() const
{
	return Sphere(a, r);
}

Sphere Capsule::GetS2() const
{
	return Sphere(b, r);
}

Cylinder Capsule::GetCylinder() const
{
	return Cylinder(a, b, r);
}
//### Capsule

//### Cylinder
Cylinder::Cylinder(P a_, P b_, double r_)
	:r(r_)
{
	a = a_;
	b = b_;
}

bool Cylinder::IsPointInside(P p) const
{
	//d 垂直距离;
	//dis 水平距离
	P d = norm(b - a);
	P inter = b + d*dot(p-b,d);
	if (dis(p, inter) > r)
	{
		return false;
	}
	double len = dis(a, b);
	double dist = dot(inter - a, b - a) / len;
	return dist >= 0 && dist <= len;
}
//### Cylinder

//### Plane
void Plane::Define(P n_, P p_)
{
	n = n_;
	p = p_;
	isDefined = true;
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
	if (d <= len(l) && d>=0)
	{
		re.result = true;
		re.d = d;
		re.hitP = l.a + d * l.dir();
	}
	return re;
}
//### Plane
//### Global Plane
P project(P p, const Plane& plane)
{
	if (!plane.isDefined)
	{
		abort();
	}
	return p - plane.n * dot(p - plane.p, plane.n);
}
//### Global Plane

//### Tri
Tri::Tri()
{

}

Tri::Tri(const P& p1_, const P& p2_, const P& p3_):
	p1(p1_),p2(p2_),p3(p3_)
{
	CalculateNormal();
	Plane::Define(n, p1);
}

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

void Tri::Scale(double s)
{
	p1 *= s;
	p2 *= s;
	p3 *= s;
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

IntersectInfo Tri::Collide(const Line& l) const
{
	IntersectInfo re;
	//1.判断与平面相交
	re = Plane::Intersect(l);
	//2.判断交点是否在三角形内
	if (re.result)
	{
		P hitP = l.a + re.d*l.dir();
		//所谓"重心法"：https://blog.csdn.net/wkl115211/article/details/80215421
		re.result = IsPointInside(hitP);
	}
	return re;
}

IntersectInfo Tri::Collide(const Cylinder& cylinder) const
{
	IntersectInfo re;
	//???
	return re;
}

IntersectInfo Tri::Collide(const Capsule& cap) const
{
	IntersectInfo re;
	//1.判断头尾球体是否与tri相交
	//对于移动中的traj形成的cap，应该不会检测到s1的相交
	IntersectInfo bs1 = Collide(cap.GetS1());
	if (bs1)
	{
		return bs1;
	}
	IntersectInfo bs2 = Collide(cap.GetS2());
	if (bs2)
	{
		double d2 = bs2.d;
		P a2 = project(cap.a, *this);
		double d1 = dis(cap.a, a2);
		bs2.hitP = lerp(cap.b, cap.a, lerpRate(d2, d1, cap.r));
		return bs2;
	}
	IntersectInfo b3 = Collide(cap.GetCylinder());
	if(b3)
	{
		return b3;
	}
	return re;
}

IntersectInfo Tri::Collide(const Sphere& s) const
{
	//判断相交的办法有很多，我用一种“杂糅法”
	//1.判断投影竖直方向上相交
	//2.判断球心和三角形的Mincowsky sum（三角面+球）是否相交，那么判断：
	//投影点在三角形内 || （点在3球||3圆柱内）

	//球/三角面相交的返回值：
	//1.球投影在平面上的圆心位置
	//2.球与平面截得的半径
	IntersectInfo re;

	re.hitP = project(s.center, *this);
	re.d = dis(s.center,re.hitP);
	re.hitR = side(s.r, re.d);
	//1.保证投影竖直方向上相交
	if (s.r <= re.d)
	{
		return re;
	}
	//2.判断Mincowsky sum
	bool b1 = IsPointInside(re.hitP);
	bool b2 = M_Add(*this,s).IsPointInside(s.center);
	re.result = b1 || b2;
	//??? debug
	if (re.result)
	{
		int aa = 1;
	}
	return re;
}
//### Tri

//### Sphere
Sphere::Sphere(double r_):r(r_)
{

}

Sphere::Sphere(P center_, double r_):
	center(center_),r(r_)
{

}

bool Sphere::Collide(const Shape* other)
{
	auto s = dynamic_cast<const Sphere*>(other);
	if (s!=nullptr)
	{
		return SphereCollide(*this, *s);
	}
	else
	{
		abort();
	}
	return false;
}

bool Sphere::IsPointInside(P p) const
{
	return dis(p, center) <= r;
}

str Sphere::TxtHeadString()
{
	str re;
	re.AddDouble(r, 5);
	return re;
}
//### Sphere

//### Global Sphere 
bool SphereCollide(const Sphere& s1, const Sphere& s2)
{
	return dis(s1.center, s2.center) < (s1.r + s2.r);
}
//### Global Sphere 

//### ShapeHull
bool ShapeHull::IsPointInside(P pos)
{
	for (auto& shape : hulls)
	{
		if (!shape->IsPointInside(pos))
		{
			return false;
		}
	}
	return true;
}
//### ShapeHull

//### Global ShapeHull
ShapeHull M_Add(const Tri& tri, const Sphere& s)
{
	ShapeHull re;
	auto& hulls = re.hulls;
	hulls += new Sphere(tri.p1, s.r);
	hulls += new Sphere(tri.p2, s.r);
	hulls += new Sphere(tri.p3, s.r);
	hulls += new Cylinder(tri.p1, tri.p2, s.r);
	hulls += new Cylinder(tri.p2, tri.p3, s.r);
	hulls += new Cylinder(tri.p3, tri.p1, s.r);
	return re;
}
//### Global ShapeHull