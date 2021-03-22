#pragma once
#include <vector>
#include <string>
using std::vector;
using std::string;

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

	T& operator[](int inx)
	{
		return data_[inx];
	}

	int size()
	{
		return data_.size();
	}

	void resize(int newLen)
	{
		data_.resize(newLen);
	}
};

class str
{
public:
	str(const char* s):data(string(s)) {};
	string data;
};