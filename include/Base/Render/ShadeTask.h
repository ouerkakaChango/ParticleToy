#pragma once
#include "Material.h"

class ShadeTask
{
public:
	ShadeTask(Material* material_, const P& p_, const P& n_, const P& v_, const arr<LightInfo>& lightInfos_);
	P Calculate();

	Material* material;
	P p, n, v;
	arr<LightInfo> lightInfos;
};