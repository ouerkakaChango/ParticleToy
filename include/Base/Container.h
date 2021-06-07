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

	void operator+=(const arr<T>& other)
	{
		for (auto& t : other.data_)
		{
			*this += t;
		}
	}

	T& operator[](int inx)
	{
		return data_[inx];
	}

	const T& operator[](int inx) const
	{
		return data_[inx];
	}

	unsigned int size() const
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

	bool Has(T t)
	{
		for (auto& iter : data_)
		{
			if (iter == t)
			{
				return true;
			}
		}
		return false;
	}

	void delAt(int inx)
	{
		data_.erase(data_.begin()+inx);
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
		data_.resize(x);
		for (int i = 0; i < x; i++)
		{
			data_[i].resize(y);
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

template<class T>
class arr3
{
public:
	void resize(int x_, int y_, int z_)
	{
		x = x_;
		y = y_;
		z = z_;
		data_.resize(z);
		for (int k = 0; k < z; k++)
		{
			data_[k].resize(x,y);
		}
	}

	arr2<T>& operator[](int inx)
	{
		return data_[inx];
	}

	arr<arr2<T>> data_;
	int x, y, z;
};

class str
{
public:
	str() :data(string("")) {}
	str(const char* s):data(string(s)) {}
	void operator+=(const str& other);
	void operator+=(double other);
	void operator+=(int other);
	bool operator==(const char* s) const;
	bool operator==(const str& str) const;
	bool operator!=(const char* s) const;
	bool operator!=(const str& str) const;
	friend ostream & operator <<(ostream & os, const str & s);
	void AddDouble(double other, int precision);
	bool Has(str s) const;
	string data;
};
ostream& operator <<(ostream & os, const str & s);
#define typeStr(obj) str(typeid(obj).name())

class int2
{
public:
	int2();
	int2(int x_, int y_);
	int2 operator+(const int2& t);
	str ToStr();
	int x=0, y=0;
};