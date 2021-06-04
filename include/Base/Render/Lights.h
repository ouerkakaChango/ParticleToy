#pragma once
#include "FastMath.h"

class LightInfo
{
public:
	P color, pos, dir;
};

class Light
{
public:
	Light(P color_);
	virtual LightInfo GetLightInfo(const P& pos)=0;
	P color;
};

class DirectionalLight : public Light
{
public:
	DirectionalLight(P dir_,P color_);
	LightInfo GetLightInfo(const P& pos) override;
	P dir;
};