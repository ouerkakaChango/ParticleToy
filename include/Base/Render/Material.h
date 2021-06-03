#pragma once
#include "Taiji.h"
#include "FastMath.h"


class Material
{
	THISY(Material)
	P Calculate(const arr<class Light*>& lights, P n, P v);
};

class MaterialI : public ClassI
{

};

class MaterialO : public ClassO
{
public:
	virtual P Calculate(MaterialI* param, const arr<class Light*>& lights, P n, P v)=0;
};

class BlinnPhongI : public MaterialI
{
public:
	P ambientColor = P(1, 1, 1);
	P diffuseColor = P(1, 1, 1);
	P specularColor = P(1, 1, 1);
	double specPower = 3.0;
	double kA = 0.05;
	double kD = 0.5;			//Diffuse intensity
	double kS = 1.0;			//Specular intensity

	//double reflectness = 0.1;		//when ray-traced,(reflectness percent of color is from bounce color)
};

class BlinnPhongO : public MaterialO
{
public:
	P Calculate(MaterialI* matParam, const arr<class Light*>& lights, P n, P v) override;
};