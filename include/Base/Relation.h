#pragma once
#include "Taiji.h"
#include "FastMath.h"

class Pnt;
class ExtraInfo;

class Relation : public Ying
{
	THISY(Relation)
	virtual P RelationForce(const Pnt& pnt, const ExtraInfo& info) = 0;
};

class RestLengthRelation :public Relation
{
	THISY(RestLengthRelation)
	RestLengthRelation(int pntInx, double restLen);

	P RelationForce(const Pnt& pnt, const ExtraInfo& info) override;
};

class RestLengthRelationI : public ClassI
{
public:
	int pntInx;
};

class RestLengthRelationO : public ClassO
{
public:
	double restLen;
};