#pragma once

#include "Render/Material.h"

class Extra_BlinnPhongI_CheapBounce : public ClassI
{
public:
	double reflectness = 0.1;		//when ray-traced,(reflectness percent of color is from bounce color)

};

class Extra_ReflectBounce : public ClassI
{
public:
	double reflectEnegyRate = 0.8;	//reflectEnegyRate percent if color is kept when consider the color as indirLight
};