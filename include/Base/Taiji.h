#pragma once
#include "Container.h"

#define THIS(className) public:\
className();\
arr<void*> i; \
arr<R*> r;

class R
{
public:
	virtual void SayI()=0;
};

class Taiji
{
	THIS(Taiji)
};

template <class T>
void SayI(T* tt_)
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