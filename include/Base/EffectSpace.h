#pragma once

#include "Taiji.h"
#include "FastMath.h"
#include "FastGeo.h"

class Pnt;
class EffectSpace : public Ying
{
	THISY(EffectSpace)
	inline bool IsDefined() { return isDefined; }
	void DefineLineI(P a, P b);
	void DefineCapsuleI(P a, P b, double r);
	IntersectInfo Collide(const Tri& tri);
	inline void SetIgnore(bool isIgnore_) { isIgnore = isIgnore_; }
	void Update(const Pnt& pnt);
	void Update(P pos)=delete;
	void SafeUpdate(const Pnt& prevPnt,Pnt& pnt);
	void ResetPrev(P p);

	bool isDefined = false;
	bool isIgnore = false;
	str type;
};

class EffectSpaceI : public ClassI
{
public:
	virtual IntersectInfo Collide(const Tri& tri);
};

class EffectLineI : public EffectSpaceI
{
public:
	void Set(P a, P b);
	void UpdateB(P newb);
	IntersectInfo Collide(const Tri& tri) override;
	Line l;
};

class EffectCapsuleI : public EffectSpaceI
{
public:
	void Set(P a, P b, double r);
	void UpdateB(P newb);
	IntersectInfo Collide(const Tri& tri) override;
	Capsule capsule;
};