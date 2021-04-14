#pragma once

#include <cmath>

template <class T>
T Cast(void* ptr)
{
	return static_cast<T>(ptr);
}

template<class T>
T Cast(double n)
{
	return static_cast<T>(n);
}

template<>
int Cast(double n);