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
	//!!! ���ڱȽ�hardcode,�Ȼ�������ô�������restForce�����Գ������
	if (dL < 0)
	{
		re += 3 * sign(dL)*norm(dP);
	}
	return re;
}
//### RestLengthRelation