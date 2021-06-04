#include "Material.h"
#include "Lights.h"

//### Material
Material::Material()
{
	//default use blinn-Phong

	i += new BlinnPhongI;
	o += new BlinnPhongO;

}

P Material::Calculate(const arr<LightInfo>& lightsInfo, P n, P v)
{
	return Cast<MaterialO*>(o[0])->Calculate(Cast<MaterialI*>(i[0]), lightsInfo, n, v);
}
//### Material

//### BlinnPhongO
P BlinnPhongO::Calculate(MaterialI* matParam, const arr<LightInfo>& lightsInfo, P n, P v)
{
	P re;
	BlinnPhongI* param = Cast<BlinnPhongI*>(matParam);
	P ambient = param->ambientColor *  param->kA;
	re += ambient;
	for (int i = 0; i < lightsInfo.size(); i++)
	{
		P l = -lightsInfo[i].dir;
		P lightColor = lightsInfo[i].color;
		P h = norm(l + v);
		double NdotL = max(0, dot(n, l));
		P diffuse = lightColor * param->diffuseColor * NdotL * param->kD;
		P spec = lightColor * param->specularColor * pow(max(0, dot(n, h)), param->specPower) * param->kS;
		re += diffuse;
		re += spec;
	}
	re = saturate(re);
	return re;
}
//### BlinnPhongO