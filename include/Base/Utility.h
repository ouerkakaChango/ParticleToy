#pragma once

#include <cmath>
#define MAX_PATH_LENGTH 100  //max filepath,used for 3-party c-type API,such as _wgetcwd

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