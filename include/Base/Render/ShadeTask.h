#pragma once
#include "Material.h"

class ShadeTask
{
public:
	ShadeTask() {}
	ShadeTask(Material* material_, const P& p_, const P& n_, const P& v_, const arr<LightInfo>& lightInfos_);
	P Calculate();

	Material* material=nullptr;
	P p, n, v;
	MaterialExtraControl* control=nullptr;
	arr<LightInfo> lightInfos;
};