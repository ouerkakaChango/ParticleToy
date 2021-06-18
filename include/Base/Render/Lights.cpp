#include "Lights.h"

//### Light
Light::Light(P color_):
	color(color_)
{

}
//### Light

//### DirectionalLight
DirectionalLight::DirectionalLight(P dir_, P color_):
	Light(color_), dir(dir_)
{

}

LightInfo DirectionalLight::GetLightInfo(const P& pos)
{
	LightInfo re;
	re.color = color;
	re.dir = dir;
	return re;
}
//### DirectionalLight

//### PointLight
PointLight::PointLight(P lightPos_, P color_, double intensity_):
	Light(color_), lightPos(lightPos_)
{

}

LightInfo PointLight::GetLightInfo(const P& pos)
{
	double ldis2 = dis2(pos, lightPos);
	double attenuation = 1.0;
	//防止距离太近的时候除爆了，衰减亮度需要一个最小值
	if (ldis2 > d2min)
	{
		attenuation = (d2max - ldis2) / (d2max - d2min);
	}
	if (attenuation > 1)
	{
		abort();
	}
	LightInfo re;
	re.color = color*attenuation;
	re.dir = safeNorm(pos-lightPos);
	return re;
}

LightInfo PointLight::GetLightInfo(const P& pos, double enengyRate)
{
	double ldis2 = dis2(pos, lightPos);
	double attenuation = 1.0;
	//防止距离太近的时候除爆了，衰减亮度需要一个最小值
	if (ldis2 > d2min)
	{
		attenuation = (d2max - ldis2) / (d2max - d2min);
	}
	if (attenuation > 1)
	{
		abort();
	}
	attenuation *= enengyRate;
	LightInfo re;
	re.color = color * attenuation;
	re.dir = safeNorm(pos - lightPos);
	return re;
}
//### PointLight