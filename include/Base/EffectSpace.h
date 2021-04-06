#pragma once

#include "Taiji.h"
#include "FastMath.h"
#include "FastGeo.h"

class IntersectInfo
{

};

class EffectSpace : public Ying
{
	THISY(EffectSpace)
	inline bool IsDefined() { return isDefined; }
	void Define(str type);
	IntersectInfo Intersect(const Tri& tri);

	bool isDefined = false;
};

class EffectSpaceI : public ClassI
{
public:
	virtual IntersectInfo Intersect(const Tri& tri);
};

class EffectLineI : public EffectSpaceI
{
public:
	void Set(P a_, P b_);
	void Update(P newb);
	IntersectInfo Intersect(const Tri& tri) override;
	P a, b;
};