#pragma once
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
using std::vector;
using std::string;
using std::ostream;

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

	const T& operator[](int inx) const
	{
		return data_[inx];
	}

	int size() const
	{
		return data_.size();
	}

	void resize(int newLen)
	{
		data_.resize(newLen);
	}

	void clear()
	{
		data_.clear();
	}

	void allset(T t)
	{
		for (auto& iter : data_)
		{
			iter = t;
		}
	}
};

template <class T>
class arr2
{
public:

	void resize(int x_, int y_)
	{
		x = x_;
		y = y_;
		data_.resize(y);
		for (int j = 0; j < y; j++)
		{
			for (int i = 0; i < x; i++)
			{
				data_[j].resize(x);
			}
		}
	}

	arr<T>& operator[](int inx)
	{
		return data_[inx];
	}

	const arr<T>& operator[](int inx) const
	{
		return data_[inx];
	}

	arr<arr<T>> data_;
	int x, y;
};

class str
{
public:
	str() :data(string("")) {}
	str(const char* s):data(string(s)) {}
	void operator+=(const str& other);
	void operator+=(double other);
	bool operator==(const char* s);
	friend ostream & operator <<(ostream & os, const str & s);
	void AddDouble(double other, int precision);
	bool Has(str s);
	string data;
};
ostream& operator <<(ostream & os, const str & s);