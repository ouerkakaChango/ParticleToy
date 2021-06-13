#include "Material.h"
#include "Lights.h"

//### Material
Material::Material()
{
	//default use blinn-Phong

	//i += new BlinnPhongI;
	//o += new BlinnPhongO;

}

P Material::Calculate(const arr<LightInfo>& lightsInfo, P n, P v, MaterialExtraControl* control)
{
	return Cast<MaterialO*>(o[0])->Calculate(Cast<MaterialI*>(i[0]), lightsInfo, n, v, control);
}
//### Material

//### MaterialO
double MaterialO::pdf(const P& n, const P& h, rayTraceSampleMode sampleMode, MaterialI* matParam)
{
	if (sampleMode == rayTraceSampleMode_UniformSampling)
	{
		return 1/(2 * PI);
	}
	abort();
	return 1.0;
}

P MaterialO::ImportanceRandSampleDir(MaterialI* matParam, const P& n, const P& v)
{
	return n;
}
//### MaterialO

//### BlinnPhongO
P BlinnPhongO::Calculate(MaterialI* matParam, const arr<LightInfo>& lightsInfo, P n, P v, MaterialExtraControl* control)
{
	P re;
	BlinnPhongI* param = Cast<BlinnPhongI*>(matParam);
	P ambient = param->ambientColor *  param->kA;
	re += ambient;
	for (int i = 0; i < lightsInfo.size(); i++)
	{
		P l = -lightsInfo[i].dir;
		P lightColor = lightsInfo[i].color;
		P h = safeNorm(l + v);
		double NdotL = max(0, dot(n, l));
		P diffuse = lightColor * param->diffuseColor * NdotL * param->kD;
		P spec = lightColor * param->specularColor * pow(max(0, dot(n, h)), param->specPower) * param->kS;
		re += diffuse;
		re += spec;
	}
	
	if (control == nullptr || !control->bIgnoreEmissive)
	{
		re += param->emissive;
	}

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

P PBRO::Calculate(MaterialI* matParam, const arr<LightInfo>& lightsInfo, P n, P v, MaterialExtraControl* control)
{
	P re;
	PBRI* param = Cast<PBRI*>(matParam);
	P ambient = P(0.03) * param->ambientRate * param->albedo * param->ao;
	re += ambient;

	for (int i = 0; i < lightsInfo.size(); i++)
	{
		P l = -lightsInfo[i].dir;
		P lightColor = lightsInfo[i].color;
		P h = safeNorm(l + v);

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
		Lo *= lightColor*max(dot(n,l),0);

		if ( control != nullptr && control->bDividePDF)
		{
			double deno = pdf(n, h, control->sampleMode, matParam);
			Lo /= deno;
		}

		re += Lo;

	}

	if (control==nullptr || !control->bIgnoreEmissive)
	{
		re += param->emissive ;
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

double PBRO::pdf(const P& n, const P& h, rayTraceSampleMode sampleMode, MaterialI* matParam)
{
	if (sampleMode == rayTraceSampleMode_UniformSampling)
	{
		return 1.0/(2.0 * PI);
	}
	else if (sampleMode == rayTraceSampleMode_ImportanceSampling)
	{
		double re;
		//https://agraphicsguy.wordpress.com/2015/11/01/sampling-microfacet-brdf/
		//关于pdf的除0问题，搜了半天也没找到合适解决办法：
		//https://stackoverflow.com/questions/8271210/upper-confidence-bounds-in-monte-carlo-tree-search-when-plays-or-visited-are-0/8273267
		//!!! 我就让pdf计算时加上小数，避免0,并且使用函数，保证func(c*s)>=1
		PBRI* param = Cast<PBRI*>(matParam);
		double a = lerp(0.00001,1.0,param->roughness * param->roughness);
		double theta = acos(dot(n, h));
		theta = lerp(0.00001, PI/2, theta/(PI/2));
		double c = cos(theta);
		double s = sin(theta);

		double nomi = a * a * c * s;
		double deno = c * c * (a*a - 1) + 1;
		deno = PI * deno * deno;
		re = nomi / deno;

		return re;
	}
	abort();
	return 1.0;
}

P PBRO::ImportanceRandSampleDir(MaterialI* matParam, const P& n, const P& v)
{
	//根据：https://agraphicsguy.wordpress.com/2015/11/01/sampling-microfacet-brdf/ ,
	//1 均撒e,phi，e根据公式生成theta
	//2 根据theta,phi用球坐标系生成向量h_local
	//4 根据n法平面调整h_local为h_world
	//5 令l = safeNorm(h-v)，如果l为0就重新撒。
	//6 沿l方向生成subray

	P dir;
	//根据目前DistributionGGX的实现,a=rougness * rougness
	PBRI* param = Cast<PBRI*>(matParam);
	double a = param->roughness * param->roughness;

	while (zero(dir))
	{
		double phi = rand01() * 2 * PI;
		double e = rand01();
		double theta = atan(a*sqrt(e / (1 - e)));
		P h_local = PFromSpherical(theta, phi);
		Q rot = QFrom(P(0, 0, 1), n);
		P h = rot.Rotate(h_local);
		auto func = [=](double t)
		{
			P p = t * h - v;
			return p.len()-1.0;
		};
		//solve l when know h,v
		double k = 0;
		bool bSolved = BisecitonSolve(func, dot(h,v), 5, k,0.0001);
		if (!bSolved)
		{
			abort();
		}
		P l = safeNorm(k * h - v);
		if (dot(l, n) > 0)
		{
			dir = l;
		}
	}

	return dir;
}
//### PBRO