#pragma once
#include <vector>
using std::vector;

template <class T>
class arr
{
public:
	vector<T> data_;
	using iter = typename vector<T>::iterator;
	iter begin()
	{
		return data_.begin();
	}
	iter end()
	{
		return data_.end();
	}

	void operator+=(T newData_)
	{
		data_.push_back(newData_);
	}

	T operator[](int inx)
	{
		return data_[inx];
	}

	int size()
	{
		return data_.size();
	}
};