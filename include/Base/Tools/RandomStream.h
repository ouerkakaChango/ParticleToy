#pragma once
#include "Taiji.h"

class RandomStream
{
THISY(RandomStream)

	template<class T>
	void Init(arr<T>* pool)
	{
		i += new RandomStreamIArray<T>(pool);
	}

};

template <class T>
class RandomStreamIArray : public ClassI
{
public:
	RandomStreamIArray(arr<T>* pool_)
	{
		pool = pool_;
	}

	arr<T>* pool;
};