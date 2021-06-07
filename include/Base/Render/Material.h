#pragma once
#include "Taiji.h"
#include "FastMath.h"
#include "Lights.h"

class MaterialExtraControl
{
public:
	bool bIgnoreEmissive = false;
};

class Material
{
	THISY(Material)
	P Calculate(const arr<LightInfo>& lightsInfo, P n, P v, MaterialExtraControl* control = nullptr);
};

class MaterialI : public ClassI
{
public:
	P emissive;
};

class MaterialO : public ClassO
{
public:
	virtual P Calculate(MaterialI* param, const arr<LightInfo>& lightsInfo, P n, P v, MaterialExtraControl* control)=0;
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
};

class BlinnPhongO : public MaterialO
{
public:
	P Calculate(MaterialI* matParam, const arr<LightInfo>& lightsInfo, P n, P v, MaterialExtraControl* control) override;
};

class PBRI : public MaterialI
{
public:
	P albedo = P(1, 1, 1);
	double ao = 1.0;
	double ambientRate = 1.0;

	double metallic = 0.0;
	double roughness = 0.8;
};

class PBRO : public MaterialO
{
public:
	P Calculate(MaterialI* matParam, const arr<LightInfo>& lightsInfo, P n, P v, MaterialExtraControl* control) override;

	P fresnelSchlick(double cosTheta, P F0);

	double DistributionGGX(const P& N, const P& H, double roughness);

	double GeometrySmith(const P& N, const P& V, const P& L, double roughness);

	double GeometrySchlickGGX(double NdotV, double roughness);
};