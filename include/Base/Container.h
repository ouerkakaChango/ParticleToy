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

	T& last()
	{
		if (size() < 1)
		{
			abort();
		}
		return data[size() - 1];
	}

	const T& last() const
	{
		if (size() < 1)
		{
			abort();
		}
		return data[size() - 1];
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

	int inxOf(T val)
	{
		for (int i = 0; i < data.size(); i++)
		{
			if (data[i] == val)
			{
				return i;
			}
		}
		return -1;
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

bool IsCharacter(char c);

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
	void operator+=(char c);
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

	char& operator[](int inx)
	{
		return data[inx];
	}

	char operator[](int inx) const
	{
		return data[inx];
	}

	char last() const
	{
		return data[data.size() - 1];
	}

	str clip(int start) const;
	str clip(char c) const;
	str clip(str startStr) const;
	str clip(int start, int size) const;

	str clipBack(char c) const;
	str clipBack(int start) const;
	str clipBack(str startStr) const;
	str clipBack(int start, int size) const;

	str range(int start, int end) const;

	int inxOf(const char& c) const;
	arr<int> indicesOf(char c) const;
	int lastInxOf(const char& c) const;

	bool beginWith(const str& s) const
	{
		return data.substr(0, s.size()) == s.data;
	}

	bool endWith(const str& last) const
	{
		return clipBack(0, last.size()) == last;
	}

	int size() const
	{
		return data.size();
	}

	str NiceSpacebar() const;

	bool isAllCharacter() const;

	arr<str> pieces(char interval) const;
	arr<str> pieces(const arr<int> intervals) const;

	string data;
};
ostream& operator <<(ostream & os, const str & s);
str operator+(const char* s, const str other);
#define typeStr(obj) str(typeid(obj).name())
str GetFileNameFromPath(const str& path);
void GetFileNameFromPath(const str& path, str& folder, str&fName, str&postfix);

class int2
{
public:
	int2();
	int2(int x_, int y_);
	int2 operator+(const int2& t);
	str ToStr();
	int x=0, y=0;
};

template <class KeyType, class ValueType>
class map
{
public:
	ValueType& operator[](KeyType key)
	{
		int inx = inxOf(key);
		if (inx >= 0)
		{
			return values[inx];
		}
		else
		{
			*this += key;
			return values.last();
		}
	}

	void add(const KeyType& key, const ValueType& val)
	{
		keys += key;
		values += val;
	}

	void operator+=(const KeyType& key)
	{
		keys += key;
		values.addEmptyElems(1);
	}

	int inxOf(KeyType key)
	{
		return keys.inxOf(key);
	}

	arr<KeyType> keys;
	arr<ValueType> values;
};

template <class KeyType, class ValueType>
class map2
{
public:
	void operator+=(const KeyType& key)
	{
		inxMap.add(key, data.size());
		data.addEmptyElems(1);
	}

	map<KeyType, ValueType>& operator[](const KeyType& key)
	{
		if (inxMap.inxOf(key) < 0)
		{
			*this += key;
			return data.last();
		}
		return data[inxMap[key]];
	}

	arr<KeyType> subKeys()
	{
		return inxMap.keys;
	}

	map<KeyType, int> inxMap;
	arr<map<KeyType, ValueType>> data;
};

template <class KeyType,class ValueType>
class map3
{
public:
	void operator+=(const KeyType& key)
	{
		inxMap.add(key, data.size());
		data.addEmptyElems(1);
	}

	map2<KeyType, ValueType>& operator[](const KeyType& key)
	{
		if (inxMap.inxOf(key)<0)
		{
			*this += key;
			return data.last();
		}
		return data[inxMap[key]];
	}

	map<KeyType, ValueType>& Inx2(const KeyType& key)
	{
		for (int& inx : inxMap.values)
		{
			map2<KeyType, ValueType>& tmap2 = data[inx];
			for (str& key2 : tmap2.inxMap.keys)
			{
				if (key2 == key)
				{
					return tmap2[key2];
				}
			}
		}
	}

	map<KeyType, int> inxMap;
	arr<map2<KeyType, ValueType>> data;
};