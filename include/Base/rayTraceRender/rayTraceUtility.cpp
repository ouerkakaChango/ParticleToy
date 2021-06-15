#include "rayTraceUtility.h"

#ifdef RAYTRACE_OpenMP

ClassRayTraceGod& ClassRayTraceGod::GetInstance()
{
	static ClassRayTraceGod god;
	return god;
}
#endif