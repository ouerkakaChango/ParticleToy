#pragma once
#include "Utility.h"
#include "Container.h"

#define THISY(className) public:\
	className();\
	arr<ClassI*> i; \
	arr<ClassO*> o; \
	arr<R*> r; \
	friend class className##R;

#define THISR(className) public:\
	className##R(className* y_) :y(y_) {}\
	void SayI() override;\
	className* y;

#define DefaultConstruct(className) \
	auto newI = new className##I; \
	i += newI; \
	auto newO = new className##O; \
	o += newO; \

class R
{
public:
	virtual void SayI()=0;
	virtual void SayO() {};
};

class ClassI;
class ClassO;
class Taiji
{
	THISY(Taiji)
};

template <class T>
void CommonSayI(T* tt_)
{
	for (auto tr : tt_->r)
	{
		tr->SayI();
	}
}

class TaijiI : public Taiji
{
};
typedef TaijiI Yang;

class TaijiO : public Taiji
{
};
typedef TaijiO Ying;

class ClassI
{
public:
	virtual void Resize(int newLen) {};
};

class ClassO 
{
public:
	virtual void Resize(int newLen) {};
};