#pragma once

#include "Render/Material.h"

class Extra_BlinnPhongI_CheapBounce : public ClassI
{
public:
	double reflectness = 0.1;		//when ray-traced,(reflectness percent of color is from bounce color)

};