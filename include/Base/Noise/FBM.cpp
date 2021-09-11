#include "FBM.h"

FBM::FBM()
{

}

double FBM::Get(P p)
{//https://thebookofshaders.com/13/

	// Initial values
	double value = 0.0;
	double amplitude = maxHeight;

	// Loop of octaves
	for (int i = 0; i < 6; i++) {
		value += amplitude * noiseFunc(p);
		p *= lacunarity;
		amplitude *= gain;
	}
	return value;
}