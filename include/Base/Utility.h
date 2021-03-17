#pragma once

template <class T>
T Cast(void* ptr)
{
	return static_cast<T>(ptr);
}