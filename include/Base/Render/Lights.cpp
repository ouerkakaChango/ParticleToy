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
	Light(color_), lightPos(lightPos_), intensity(intensity_)
{

}

LightInfo PointLight::GetLightInfo(const P& pos)
{
	double ldis2 = dis2(pos, lightPos);
	double attenuation = min(1,intensity*10 / ldis2);
	LightInfo re;
	re.color = color*attenuation;
	re.dir = safeNorm(pos-lightPos);
	return re;
}

LightInfo PointLight::GetLightInfo(const P& pos, double enengyRate)
{
	double ldis2 = dis2(pos, lightPos);
	double attenuation = enengyRate * min(1, intensity * 10 / ldis2);
	LightInfo re;
	re.color = color * attenuation;
	re.dir = safeNorm(pos - lightPos);
	return re;
}
//### PointLight