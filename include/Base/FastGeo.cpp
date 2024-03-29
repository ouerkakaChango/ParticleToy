#include "FastGeo.h"

//### Shape
int Shape::uid_count=0;

Shape::Shape()
{
	uid_count += 1;
	uid = uid_count - 1;
}

double Shape::SDF(P pos)
{
	return maxSDF;
}

P Shape::SDFNormal(P p)
{
	return safeNorm(P(
		SDF(P(p.x + epsilon, p.y, p.z)) - SDF(P(p.x - epsilon, p.y, p.z)),
		SDF(P(p.x, p.y + epsilon, p.z)) - SDF(P(p.x, p.y - epsilon, p.z)),
		SDF(P(p.x, p.y, p.z + epsilon)) - SDF(P(p.x, p.y, p.z - epsilon))
	));
}

IntersectInfo Shape::Collide(const Shape& other)
{
	IntersectInfo re;
	return re;
}

bool Shape::IsPointInside(P pos) const
{
	return false;
}

bool Shape::InsideOf(const Shape& other) const
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
Line::Line()
{

}

Line::Line(P a_, P b_):a(a_),b(b_)
{

}

void Line::Set(P a_, P b_)
{
	a = a_;
	b = b_;
}

void Line::UpdateB(P newb)
{
	a = b;
	if (equal(a.y, 0.9986434))
	{
		int aa = 1;
	}
	b = newb;
}

P Line::dir() const
{
	return norm(b - a);
}

bool Line::LineNearlyZero() const
{
	return zero((b - a).len());
}
//### Line

//### Global Line
double len(const Line& l)
{
	return len(l.b - l.a);
}
double DisToLine(P p, const Line& l)
{
	//取三角形，叉乘后得平行四边形面积再除以底边长得高
	P v1 = l.a - p;
	P v2 = l.b - p;
	P v3 = l.b - l.a;
	if (zero(v3.len()))
	{
		abort();
	}
	return cross(v1, v2).len() / v3.len();
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

bool Cylinder::IsPointInside(P pos) const
{
	if (equal(a, b))
	{
		return false;
	}
	//d 垂直距离;
	//dis 水平距离
	P d = norm(b - a);
	P inter = b + d*dot(pos - b,d);
	if (dis(pos, inter) > r)
	{
		return false;
	}
	double len = dis(a, b);
	double dist = dot(inter - a, b - a) / len;
	return dist >= 0 && dist <= len;
}
//### Cylinder

//### Plane
Plane::Plane()
{

}

Plane::Plane(P n_, P p_):
	n(n_),p(p_),isDefined(true)
{

}

Plane::Plane(const Plane& plane):
	n(plane.n),p(plane.p),isDefined(plane.isDefined)
{

}
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

double Plane::sdf(P pos) const
{
	return dot(n, pos - p);
}

bool Plane::IsPointUnder(P pos, const Shape* outer) const
{
	double r = 0;
	if (outer == nullptr)
	{
		//r=0
	}
	else if (typeStr(*outer) == "class Sphere")
	{
		r = static_cast<const Sphere*>(outer)->r;
	}
	else
	{
		abort();
	}
	return sdf(pos) - r < 0;
}

P Plane::GetFixedPos(P pos, const Shape* outer) const
{

	if (outer == nullptr)
	{
		return pos;
	}
	double r=0;
	P pj = project(pos, *this);
	if (typeStr(*outer) == "class Sphere")
	{
		double r = static_cast<const Sphere*>(outer)->r;
		return pj + n * r;
	}
	else
	{
		abort();
	}
	return pos;
}

bool Plane::IsPointInside(P pos) const
{
	return equal(sdf(pos), 0);
}

bool Plane::IsPointFixed(P pos, const Shape* outer) const
{
	if (outer == nullptr)
	{
		return IsPointInside(pos);
	}
	if (typeStr(*outer) == "class Sphere")
	{
		double r = static_cast<const Sphere*>(outer)->r;
		return equal(sdf(pos), r);
	}
	else
	{
		abort();
	}
	return false;
}
//### Plane

//### Global Plane
P project(P pos, const Plane& plane)
{
	if (!plane.isDefined)
	{
		abort();
	}
	return pos - plane.n * dot(pos - plane.p, plane.n);
}

P project(P dirBeforeProj, double lenBeforeProj, P dirAfterProj)
{
	return dirAfterProj * lenBeforeProj / (dot(dirBeforeProj, dirAfterProj));
}
//### Global Plane

//### Tri
Tri::Tri()
{

}

Tri::Tri(const Tri& tri):Plane(tri)
{
	p1 = tri.p1;
	p2 = tri.p2;
	p3 = tri.p3;
	uid = tri.uid;
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

bool Tri::IsPointInside(P pos) const
{
	P v0 = p3 - p1;
	P v1 = p2 - p1;
	P v2 = pos - p1;

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
	re = Intersect(l,*this);
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
	if (cylinder.LineNearlyZero())
	{
		return re;
	}
	//https://blog.csdn.net/qq_41524721/article/details/116141905?spm=1001.2014.3001.5501
	//步骤一 3点在cylinder内测试
	bool b1 = cylinder.IsPointInside(p1);
	bool b2 = cylinder.IsPointInside(p2);
	bool b3 = cylinder.IsPointInside(p3);
	if (b1 || b2 || b3)
	{
		//还未补全
		abort();
		re.result = true;
		return re;
	}
	//步骤二 3边与cylinder相交
	IntersectInfo e1 = Intersect(Line(p1, p2),cylinder);
	IntersectInfo e2 = Intersect(Line(p2, p3),cylinder);
	IntersectInfo e3 = Intersect(Line(p3, p1),cylinder);
	if (e1 || e2 || e3)
	{
		re = e1 ? e1 : (e2 ? e2 : e3);
		return re;
	}

	//步骤三 圆柱轴线和三角面相交测试
	//这个Intersect保证交点在线段上，但不保证在三角形里
	IntersectInfo c = Intersect(cylinder, *this);
	if (c.result && IsPointInside(c.hitP))
	{
		//还未补全
		abort();
		re.result = true;
		return re;
	}

	//!!! 现在都是开口的圆柱体，如果是封口的，还要加上步骤四
	return re;
}

IntersectInfo Tri::Collide(const Capsule& cap) const
{
	//返回的hitP是S2球心移到平面之上的位置
	IntersectInfo re;

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
	if (b3)
	{
		P cen = b3.hitP;
		P dir = -cap.dir();
		b3.hitP = RayIntersect(cap,*this).hitP + project(n, cap.r, dir);
		return b3;
	}
	IntersectInfo bs1 = Collide(cap.GetS1());
	if (bs1)
	{
		//当prev是virtual的，可能会进这个分支
		double d2 = bs1.d;
		P a2 = project(cap.a, *this);
		double d1 = dis(cap.a, a2);
		bs1.hitP = lerp(cap.b, cap.a, lerpRate(d2, d1, cap.r));
		return bs1;
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
	//!!! 以前side写的有问题，这边好像也没出错？逻辑问题一直没管
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

double Sphere::SDF(P pos)
{
	return dis(pos, center) - r;
}

P Sphere::SDFNormal(P pos)
{
	return norm(pos - center);
}

IntersectInfo Sphere::Collide(const Shape& other)
{
	IntersectInfo re;
	if (typeStr(other)=="class Sphere")
	{
		//如果相交，hitP为两圆心中点
		//d为需要两圆心一共向外移动的距离
		auto& s = static_cast<const Sphere&>(other);
		re.result = SphereCollide(*this, s);
		if (re.result)
		{
			re.hitP = (center + s.center) / 2;
			re.d = s.r + r - dis(center, s.center);
			re.r1 = r;
			re.r2 = s.r;
		}
	}
	else
	{
		abort();
	}
	return re;
}

bool Sphere::IsPointInside(P pos) const
{
	return dis(pos, center) <= r;
}

bool Sphere::InsideOf(const Shape& other) const
{
	if (typeStr(other) == "class Sphere")
	{
		auto& s = static_cast<const Sphere&>(other);
		return dis(center, s.center) + r < s.r;
	}
	abort();
	return false;
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

//### Box
Box::Box(P center_, P bound)
	:center(center_), half(bound/2)
{

}

double Box::SDF(P p)
{
	P q = abs(p-center) - half;
	return len(max(q, 0.0)) + min(max(q.x, max(q.y, q.z)), 0.0);
}
//### Box

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

//### Global
IntersectInfo Intersect(const Line& line, const Cylinder& cylinder)
{
	IntersectInfo re;
	if (cylinder.LineNearlyZero())
	{
		return re;
	}
	//懒得完全推导，先用二分法解方程求得线段上的交点之一
	auto func = [=](double t)
	{
		P p = line.a + t * (line.b - line.a);
		return DisToLine(p, cylinder) - cylinder.r;
	};
	double x1 = -1.0;
	bool bSolve1 = BisecitonSolve(func, 0, 1, x1);
	auto func2 = [=](double t)
	{
		P p = line.b + t * (line.a - line.b);
		return DisToLine(p, cylinder) - cylinder.r;
	};
	//由于是2分法，如果“允许解域”上有2个解，从另一个端点开始解，能得到第二个解
	double x2 = -1.0;
	bool bSolve2 = BisecitonSolve(func2, 0, 1, x2);
	if (!bSolve1 && !bSolve2)
	{
		return re;
	}
	//判断交点是否在cylinder内
	P p1 = line.a + x1 * (line.b - line.a);
	bool b1 = cylinder.IsPointInside(p1);
	P p2 = line.b + x2 * (line.a - line.b);
	bool b2 = cylinder.IsPointInside(p2);
	if (b1 || b2)
	{
		re.result = true;
		if (b1&&b2)
		{
			P cen = (p1 + p2) / 2;
			re.hitP = cen;
		}
		else
		{
			abort();
		}
	}
	return re;
}

IntersectInfo Intersect(const Line& line, const Plane& plane)
{
	IntersectInfo re;
	if (line.LineNearlyZero())
	{
		return re;
	}
	//原理：https://blog.csdn.net/qq_41524721/article/details/103490144
	double d = -dot(line.a - plane.p, plane.n) / dot(line.dir(), plane.n);
	if (d <= len(line) && d >= 0)
	{
		re.result = true;
		re.d = d;
		re.hitP = line.a + d * line.dir();
	}
	return re;
}

IntersectInfo RayIntersect(const Line& line, const Plane& plane)
{
	IntersectInfo re;
	if (line.LineNearlyZero())
	{
		return re;
	}
	double d = -dot(line.a - plane.p, plane.n) / dot(line.dir(), plane.n);
	re.hitP = line.a + d * line.dir();
	re.result = true;
	return re;
}
//### Global