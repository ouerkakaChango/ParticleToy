#pragma once
#include "Taiji.h"
#include "FastMath.h"
#include "Lights.h"

enum rayTraceSampleMode
{
	rayTraceSampleMode_UniformSampling,
	rayTraceSampleMode_ImportanceSampling,
};

class MaterialExtraControl
{
public:
	bool bIgnoreEmissive = false;
	bool bDividePDF = false;
	rayTraceSampleMode sampleMode = rayTraceSampleMode_UniformSampling;
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
	virtual P Calculate(MaterialI* matParam, const arr<LightInfo>& lightsInfo, P n, P v, MaterialExtraControl* control)=0;
	//由于参考:https://agraphicsguy.wordpress.com/2015/11/01/sampling-microfacet-brdf/
	//光追重要性采样时会根据渲染方程的不同而用相应推导出的pdf，所以pdf实现放material里比较合适
	virtual double pdf(const P& n, const P& h, rayTraceSampleMode sampleMode, MaterialI* matParam=nullptr);
	//同pdf理，重要性采样的采样方式放material里比较合适
	virtual P ImportanceRandSampleDir(MaterialI* matParam, const P& n, const P& v);
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
	double ambientRate = 0.0;

	double metallic = 0.4;
	double roughness = 0.5;
};

class PBRO : public MaterialO
{
public:
	P Calculate(MaterialI* matParam, const arr<LightInfo>& lightsInfo, P n, P v, MaterialExtraControl* control) override;

	P fresnelSchlick(double cosTheta, P F0);

	double DistributionGGX(const P& N, const P& H, double roughness);

	double GeometrySmith(const P& N, const P& V, const P& L, double roughness);

	double GeometrySchlickGGX(double NdotV, double roughness);

	double pdf(const P& n, const P& h, rayTraceSampleMode sampleMode, MaterialI* matParam = nullptr) override;
	P ImportanceRandSampleDir(MaterialI* matParam, const P& n, const P& v) override;
};