#pragma once
#include "Taiji.h"
#include "FastMath.h"

class Relation : public Ying
{
	THISY(Relation)
};

class RestRelation :public Relation
{
	THISY(RestRelation)
	RestRelation(int pntInx,P restPos);
};

class RestRelationI : public ClassI
{
public:
	int pntInx;
};

class RestRelationO : public ClassO
{
public:
	P restPos;
};