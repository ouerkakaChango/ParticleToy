#include "rayTraceUtility.h"

ClassRayTraceGod& ClassRayTraceGod::GetInstance()
{
	static ClassRayTraceGod god;
	return god;
}