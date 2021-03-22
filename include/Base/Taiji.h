#pragma once
#include "Utility.h"
#include "Container.h"

#define THISY(className) public:\
	className();\
	arr<ClassI*> i; \
	arr<R*> r;

#define THISR(className) public:\
	className##R(className* y_) :y(y_) {}\
	void SayI() override;\
	className* y;
class ClassI;
class R
{
public:
	virtual void SayI()=0;
};

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
public:
};
typedef TaijiO Ying;

class ClassI
{
public:
	virtual void Resize(int newLen) = 0;
};