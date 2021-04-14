#include "Utility.h"

template<>
int Cast(double n)
{
	int re = static_cast<int>(n);
	double f = n - re;
	if (std::abs(f) > 0.00001)
	{
		abort();
	}
	return re;
}