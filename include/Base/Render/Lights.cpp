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