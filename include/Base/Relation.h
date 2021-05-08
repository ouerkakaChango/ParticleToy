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

	double forceRate = 3.0;
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

class RestPosRelation : public Relation
{
	THISY(RestPosRelation)
	RestPosRelation(int pntInx, P restPos, double forceRate_=3.0);

	P RelationForce(const Pnt& pnt, const ExtraInfo& info) override;

	double forceRate=3.0;
};

class RestPosRelationI : public ClassI
{
public:
	int pntInx;
};

class RestPosRelationO : public ClassO
{
public:
	P restPos;
};