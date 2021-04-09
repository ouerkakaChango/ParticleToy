#pragma once

#include "Taiji.h"
#include "FastMath.h"
#include "FastGeo.h"

class EffectSpace : public Ying
{
	THISY(EffectSpace)
	inline bool IsDefined() { return isDefined; }
	void DefineLineI(P a_, P b_);
	IntersectInfo Intersect(const Tri& tri);
	inline void SetIgnore(bool isIgnore_) { isIgnore = isIgnore_; }
	void Update(P p);
	void SafeUpdate(P prevP, P p);
	void ResetPrev(P p);

	bool isDefined = false;
	bool isIgnore = false;
	str type;
};

class EffectSpaceI : public ClassI
{
public:
	virtual IntersectInfo Intersect(const Tri& tri);
};

class EffectLineI : public EffectSpaceI
{
public:
	void Set(P a, P b);
	void UpdateA(P newb);
	void UpdateB(P newb);
	IntersectInfo Intersect(const Tri& tri) override;
	Line l;
};