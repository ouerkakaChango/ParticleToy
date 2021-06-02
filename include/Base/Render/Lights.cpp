#include "Lights.h"

Light::Light(P dir_, P color_):
	dir(dir_),color(color_)
{

}

DirectionalLight::DirectionalLight(P dir_, P color_):
	Light(dir_,color_)
{

}