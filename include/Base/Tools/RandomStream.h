#pragma once
#include "Container.h"
#include "FastMath.h"

template <class T>
T randElem(const arr<T> pool)
{
	return pool[randInt(pool.size())];
}

template<class T>
class RandomStream
{
public:

	void Init(arr<T>* pool_)
	{
		pool = pool_;
	}

	T Get()
	{
		return randElem(*pool);
	}

	arr<T>* pool=nullptr;
};