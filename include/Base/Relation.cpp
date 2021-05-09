#include "Relation.h"

#include "Solvers/SolverUtility.h"

//### Relation
Relation::Relation()
{

}
//### Relation

//### RestLengthRelation
RestLengthRelation::RestLengthRelation()
{
	DefaultConstruct(RestLengthRelation);
}

RestLengthRelation::RestLengthRelation(int pntInx, double restLen)
{
	auto newI = new RestLengthRelationI;
	newI->pntInx = pntInx;
	i += newI; 
	auto newO = new RestLengthRelationO;
	newO->restLen = restLen;
	o += newO; 
}

P RestLengthRelation::RelationForce(const Pnt& pnt, const ExtraInfo& info)
{
	P re;
	int otherInx = Cast<RestLengthRelationI*>(i[0])->pntInx;
	double restLen = Cast<RestLengthRelationO*>(o[0])->restLen;
	auto& other = (*info.prevPnts)[otherInx];
	P dP = other.pos - pnt.pos;
	double nowLen = dP.len();
	double dL = nowLen - restLen;
	if (dL < 0)
	{
		re += forceRate * sign(dL)*norm(dP);
	}
	return re;
}
//### RestLengthRelation

//### RestPosRelation
RestPosRelation::RestPosRelation()
{
	DefaultConstruct(RestPosRelation);
}

RestPosRelation::RestPosRelation(int pntInx, P restPos, double forceRate_):
	forceRate(forceRate_)
{
	auto newI = new RestPosRelationI;
	newI->pntInx = pntInx;
	i += newI;
	auto newO = new RestPosRelationO;
	newO->restPos = restPos;
	o += newO;
}

P RestPosRelation::RelationForce(const Pnt& pnt, const ExtraInfo& info)
{
	P re;
	int otherInx = Cast<RestPosRelationI*>(i[0])->pntInx;
	P restPos = Cast<RestPosRelationO*>(o[0])->restPos;
	auto& other = (*info.prevPnts)[otherInx];
	//{
	//	P dP = other.pos - pnt.pos;
	//	double restL = restPos.len();
	//	double l = dot(dP, restPos) / restL;
	//	if (l < restL)
	//	{
	//		re += forceRate * sign(l) * norm(-restPos);
	//	}
	//}
	P targetPos = other.pos - restPos;
	re += (targetPos - pnt.pos)*forceRate;
	return re;
}
//### RestPosRelation