#include "Relation.h"

//### Relation
Relation::Relation()
{

}
//### Relation

//### RestRelation
RestRelation::RestRelation()
{
	DefaultConstruct(RestRelation);
}

RestRelation::RestRelation(int pntInx, P restPos)
{
	auto newI = new RestRelationI;
	newI->pntInx = pntInx;
	i += newI; 
	auto newO = new RestRelationO;
	newO->restPos = restPos;
	o += newO; 
}
//### RestRelation