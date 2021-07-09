#pragma once

#include <direct.h>
#include <cmath>
#define MAX_PATH 100

template <class T>
inline T Cast(void* ptr)
{
	return static_cast<T>(ptr);
}

template<class T>
inline T Cast(double n)
{
	return static_cast<T>(n);
}