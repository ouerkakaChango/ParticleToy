#include "FracNoise.h"

FracNoise::FracNoise()
{

}

double FracNoise::Get(const P& p)
{
	//https://thebookofshaders.com/13/
	return frac(sin(dot(p, seed_dir))*
		seed_wScale);
}

double fracNoise(const P& p)
{
	FracNoise n;
	return n.Get(p);
}