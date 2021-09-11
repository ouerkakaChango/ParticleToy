#include "CellNoise.h"

#include <iostream>

P defaultSmoothFunc(const P& p)
{
	return smooth3(p);
}

CellNoise::CellNoise()
{

}

double CellNoise::Get(const P& st)
{//https://thebookofshaders.com/13/
	P i = floor(st);
	P f = frac(st);
	
	double c1 = cornerFunc(i);
	double c2 = cornerFunc(i + P(1, 0, 0));
	double c3 = cornerFunc(i + P(1, 1, 0));
	double c4 = cornerFunc(i + P(0, 1, 0));
	double c5 = cornerFunc(i + P(0, 0, 1));
	double c6 = cornerFunc(i + P(1, 0, 1));
	double c7 = cornerFunc(i + P(1, 1, 1));
	double c8 = cornerFunc(i + P(0, 1, 1));
	
	P uvw = smoothFunc(f);
	
	double lerpx1 = lerp(c1, c2, uvw.x);
	double lerpx2 = lerp(c4, c3, uvw.x);
	double lerpy1 = lerp(lerpx1, lerpx2, uvw.y);
	
	double lerpx3 = lerp(c5, c6, uvw.x);
	double lerpx4 = lerp(c8, c7, uvw.x);
	double lerpy2 = lerp(lerpx3, lerpx4, uvw.y);
	
	double lerpz = lerp(lerpy1, lerpy2, uvw.z);
	
	return lerpz;
}

double cellNoise(const P& p)
{
	CellNoise n;
	return n.Get(p);
}