#pragma once
#include "FastMath.h"

class Light
{
public:
	Light(P dir_, P color_);

	P dir;
	P color;
};

class DirectionalLight : public Light
{
public:
	DirectionalLight(P dir_,P color_);
};