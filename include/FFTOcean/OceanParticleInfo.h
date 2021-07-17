#pragma once

#include "FastMath.h"

//some params from FFT ocean equations:
//https://www.slideshare.net/Codemotion/an-introduction-to-realistic-ocean-rendering-through-fft-fabio-suriano-codemotion-rome-2017

class OceanParticleInfo
{
public:
	OceanParticleInfo(double height_=0.0);

	double height;
	cplx ksi;			
};