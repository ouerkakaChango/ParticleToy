#pragma once
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <initializer_list>
using std::vector;
using std::string;
using std::ostream;
using std::initializer_list;

template <class T>
class arr
{
public:
	arr() {}
	arr(initializer_list<T> list)
	{
		for (auto& i : list)
		{
			data.push_back(i);
		}
	}


	vector<T> data;
	using iter = typename vector<T>::iterator;
	iter begin()
	{
		return data.begin();
	}
	iter end()
	{
		return data.end();
	}

	void operator+=(T newData_)
	{
		data.push_back(newData_);
	}

	void operator+=(const arr<T>& other)
	{
		for (auto& t : other.data)
		{
			*this += t;
		}
	}

	inline T& operator[](int inx)
	{
		return data[inx];
	}

	inline const T& operator[](int inx) const
	{
		return data[inx];
	}

	unsigned int size() const
	{
		return data.size();
	}

	void resize(int newLen)
	{
		data.resize(newLen);
	}

	void clear()
	{
		data.clear();
	}

	void allset(T t)
	{
		for (auto& iter : data)
		{
			iter = t;
		}
	}

	bool Has(T t)
	{
		for (auto& iter : data)
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
		data.erase(data.begin()+inx);
	}

	void addEmptyElems(int num)
	{
		for (int i = 0; i < num; i++)
		{
			T t;
			data.push_back(t);
		}
	}
};

template <class T>
class arr2
{
public:
	inline bool empty() { return x == 0 && y == 0; }
	void resize(int x_, int y_)
	{
		x = x_;
		y = y_;
		data.resize(x);
		for (int i = 0; i < x; i++)
		{
			data[i].resize(y);
		}
	}

	arr<T>& operator[](int inx)
	{
		return data[inx];
	}

	const arr<T>& operator[](int inx) const
	{
		return data[inx];
	}

	arr<arr<T>> data;
	int x=0, y=0;
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
		data.resize(x);
		for (int i = 0; i < x; i++)
		{
			data[i].resize(y,z);
		}
	}

	arr2<T>& operator[](int inx)
	{
		return data[inx];
	}

	arr<arr2<T>> data;
	int x, y, z;
};

class str
{
public:
	str() :data(string("")) {}
	str(const string& data_) :data(data_) {}
	str(const char* s):data(string(s)) {}
	str(int n);
	str operator+(const char* s) const;
	str operator+(const str& other) const;
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

	inline const char* c_str()
	{
		return data.c_str();
	}

	str clipBack(int start,int size) const
	{
		int len = data.size();
		return data.substr(len-1- start - size, size);
	}

	string data;
};
ostream& operator <<(ostream & os, const str & s);
str operator+(const char* s, const str other);
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