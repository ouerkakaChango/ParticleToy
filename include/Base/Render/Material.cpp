#include "Material.h"
#include "Lights.h"

//### Material
Material::Material()
{
	//default use blinn-Phong

	//i += new BlinnPhongI;
	//o += new BlinnPhongO;

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

//### PBRO
P PBRO::fresnelSchlick(double cosTheta, P F0)
{
	return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

double PBRO::DistributionGGX(const P& N, const P& H, double roughness)
{
	double a = roughness * roughness;
	double a2 = a * a;
	double NdotH = max(dot(N, H), 0.0);
	double NdotH2 = NdotH * NdotH;

	double nom = a2;
	double denom = (NdotH2 * (a2 - 1.0) + 1.0);
	denom = PI * denom * denom;

	return nom / denom;
}

P PBRO::Calculate(MaterialI* matParam, const arr<LightInfo>& lightsInfo, P n, P v)
{
	P re;
	PBRI* param = Cast<PBRI*>(matParam);
	P ambient = P(0.03) * param->ambientRate * param->albedo * param->ao;
	re += ambient;

	for (int i = 0; i < lightsInfo.size(); i++)
	{
		P l = -lightsInfo[i].dir;
		P lightColor = lightsInfo[i].color;
		P h = norm(l + v);

		//Calculate F
		P F0 = P(0.04);
		F0 = lerp(F0, param->albedo, param->metallic);
		P F = fresnelSchlick(max(dot(h, v), 0.0), F0);

		//Calculate diffuse
		P kD = 1.0 - F;
		P diffuse = kD * param->albedo / PI;

		//Calculate specular
		double NDF = DistributionGGX(n, h, param->roughness);
		double G = GeometrySmith(n, v, l, param->roughness);
		P nominator = NDF * G * F;
		double denominator = 4.0 * max(dot(n, v), 0.0) * max(dot(n, l), 0.0) + 0.001;
		P specular = nominator / denominator;

		P Lo = diffuse + specular;
		Lo *= lightColor*dot(n,l);

		re += Lo;
	}
	return re;
}


double PBRO::GeometrySmith(const P& N, const P& V, const P& L, double roughness)
{
	double NdotV = max(dot(N, V), 0.0);
	double NdotL = max(dot(N, L), 0.0);
	double ggx2 = GeometrySchlickGGX(NdotV, roughness);
	double ggx1 = GeometrySchlickGGX(NdotL, roughness);

	return ggx1 * ggx2;
}

double PBRO::GeometrySchlickGGX(double NdotV, double roughness)
{
	double r = (roughness + 1.0);
	double k = (r*r) / 8.0;

	double nom = NdotV;
	double denom = NdotV * (1.0 - k) + k;

	return nom / denom;
}
//### PBRO