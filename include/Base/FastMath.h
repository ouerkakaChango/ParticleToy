#pragma once
#include "Container.h"

#include <functional>

class P2
{
public:
	P2() {}
	P2(int x_, int y_);
	P2(double x_, double y_);
	P2 operator+(const P2& p) const;
	P2 operator-(const P2& p) const;
	P2 operator-() const;
	P2 operator*(double s) const;
	P2 operator/(double s) const;
	P2 operator/(const P2& p) const;
	void operator+=(const P2& p);
	void operator/=(double s);
	bool operator>=(const P2& p) const;
	bool operator<(const P2& p) const;
	bool operator==(const P2& p) const;
	inline int IntX() const { return static_cast<int>(x); }
	inline int IntY() const { return static_cast<int>(y); }
	inline int RoundX() const { return static_cast<int>(x + 0.5); }
	inline int RoundY() const { return static_cast<int>(y + 0.5); }
	str ToStr();
	double x=0.0, y=0.0;
};

double dot(P2 p1, P2 p2);
double len(const P2& p);
P2 safeNorm(const P2& p);

class P
{
public:
	P();
	P(int n) = delete;//防止 auto p=(1,0,0)使用了括号计算器的写法
	P(double n);
	P(double x_, double y_, double z_);
	P(int x_, int y_, int z_);
	P(P2 p2, str filter);
	double x = 0.0, y = 0.0, z = 0.0;

	P operator+(const P& p) const;
	P operator-(const P& p) const;
	P operator-() const;
	P operator*(double s) const;
	P operator*(P p) const;
	P operator/(double s) const;
	P operator/(const P& p) const;
	void operator+=(const P& p);
	void operator-=(const P& p);
	void operator*=(double s);
	void operator*=(const P& p);
	void operator/=(double s);
	friend P operator*(double s, const P& p);

	double len() const;

	str ToStr();
	str ToStr(int precision);
};

P operator-(double s, const P& p);
P operator*(double s, const P& p);
P cross(const P& p1, const P& p2);
double dot(const P& p1, const P& p2);
double len(const P& p);
P norm(const P& p);
P safeNorm(const P& p);
P reflect(const P& i, const P& n);
double dis(const P& p1, const P& p2);
double dis2(const P& p1, const P& p2);
double saturate(double n);
P saturate(const P& p);
P abs(const P& p);
P max(const P& p, double n);
P max(double n, const P& p);
P min(const P& p, double n);
P min(double n, const P& p);
P pow(const P& p1, const P& p2);
P randP();
P diskRandP();
P PFromSpherical(double theta, double phi, double r=1.0);

//只接受归一化的Quaternion
//四元数相关公式 https://blog.csdn.net/silangquan/article/details/39008903
class Q
{
public:
	Q() {}
	Q(double x_, double y_,double z_, double w_);
	Q(const P& axis, double theta);
	Q operator*(const Q& q) const;
	Q Conjugate() const;
	P Rotate(const P& v) const;
	P Axis() const;
	double x, y, z, w;
};

Q QFrom(const P& vecFrom, const P& vecTo);

class cplx
{
public:
	cplx() {};
	cplx(double real_, double img_);
	cplx operator+(const cplx& c2);
	cplx operator*(double n) const;
	cplx operator*(const cplx& c2) const;
	double real=0.0, img=0.0;
};
cplx operator*(double n, const cplx& c);
cplx e_cplx(double w, double A=1.0);
cplx conju(const cplx& c);

double clamp(double x, double low, double high);
double max(double a, double b);
double min(double a, double b);
double sign(double n);
double rand01();
int randInt(int b);
double rand_norm(double mean, double standardDeviation);
bool equal(double a, double b, double tolerance = 0.00001);
bool equal(P a, P b, double tolerance = 0.00001);
bool zero(double n);
bool zero(const P2& p);
bool zero(const P& p);
double side(double hypotenuse, double side1);
template <class T,class T2>
T lerp(T a, T b, T2 k)
{
	return a + k * (b - a);
}
template <class T, class T2>
T lerpRate(T a, T b, T2 re,T tolerance=0.000001)
{
	//a+k*(b-a)=re
	if (equal(a, b, tolerance))
	{
		return 0;
	}
	return (re - a) / (b - a);
}

double lerp3D(arr<double> nums, P uvw);

bool SolveQuadra(double a, double b, double c, double& x1, double& x2);
bool SolveQuadra(P a, P b, P c, double& x1, double& x2);
double QuadraFiliter(double x1, double x2, double xmin, double xmax);
bool BisecitonSolve(std::function<double(double)> func, double tmin, double tmax, double& t, double tolerance = 0.000001);

int ToDecimal(const int* arr, int n);

int ModInx(double x, double cellLength);
P ModInx(P p, P cellLength);

double smooth5(double t);
P smooth5(P t);

#define PI 3.1415926