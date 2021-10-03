#include "Container.h"

//### global
bool IsCharacter(char c)
{
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}
//### global

//### str
str::str(int n)
{
	data = std::to_string(n);
}

str str::operator+(const char* s) const
{
	str re = *this;
	re+=s;
	return re;
}

str str::operator+(const str& other) const
{
	str re = *this;
	re += other;
	return re;
}


void str::operator+=(const str& other)
{
	data += other.data;
}

void str::operator+=(double other)
{
	data += std::to_string(other);
}

void str::operator+=(int other)
{
	data += std::to_string(other);
}

void str::operator+=(char c)
{
	data = data + c;
}

bool str::operator==(const char* s) const
{
	return data == string(s);
}

bool str::operator==(const str& str) const
{
	return data == str.data;
}

bool str::operator!=(const char* s) const
{
	return data != string(s);
}

bool str::operator!=(const str& str) const
{
	return data != str.data;
}

void str::AddDouble(double other, int precision)
{
	std::stringstream ss;
	ss << std::setiosflags(std::ios::fixed) << std::setprecision(precision) << other;
	data += ss.str();
}

bool str::Has(str s) const
{
	return data.find(s.data) != std::string::npos;
}

str str::clip(int start) const
{
	return data.substr(start, data.size() - start);
}

str str::clip(char c) const
{
	if (data[0] != c)
	{
		abort();
	}
	return clip(1);
}

str str::clip(str startStr) const
{
	return clip(startStr.size());
}

str str::clip(int start, int size) const
{
	return data.substr(start, size);
}

str str::clipBack(char c) const
{
	if (last() != c)
	{
		abort();
	}
	return clipBack(1);
}

str str::clipBack(int start) const
{
	int len = data.size();
	return data.substr(0, len - start);
}

str str::clipBack(str startStr) const
{
	return clipBack(startStr.size());
}

str str::clipBack(int start, int size) const
{
	int len = data.size();
	return data.substr(len - start - size, size);
}

str str::range(int start, int end) const
{
	return data.substr(start, end - start + 1);
}

int str::inxOf(const char& c) const
{
	for (int i = 0; i < data.size(); i++)
	{
		if (data[i] == c)
		{
			return i;
		}
	}
	return -1;
}

arr<int> str::indicesOf(char c) const
{
	arr<int> re;
	for (int i = 0; i < data.size(); i++)
	{
		if (data[i] == c)
		{
			re += i;
		}
	}
	return re;
}

int str::lastInxOf(const char& c) const
{
	int re = -1;
	for (int i = 0; i < data.size(); i++)
	{
		if (data[i] == c)
		{
			re = i;
		}
	}
	return re;
}

str str::NiceSpacebar() const
{
	str re;
	int i = 0;
	while (data[i] == ' ')
	{
		i += 1;
	}
	bool nowSpace = false;
	for (int j = i; j < data.size(); j++)
	{
		char c = data[j];
		if (!nowSpace)
		{
			re += c;
		}
		else if (c != ' ')
		{
			re += c;
			nowSpace = false;
		}
		if (c == ' ')
		{
			nowSpace = true;
		}
	}
	if (re[re.size() - 1] == ' ')
	{
		re = re.clipBack(' ');
	}
	return re;
}

bool str::isAllCharacter() const
{
	for (int i = 0; i < data.size(); i++)
	{
		if (!IsCharacter(data[i]))
		{
			return false;
		}
	}
	return true;
}

arr<str> str::pieces(char interval) const
{
	arr<str> re;
	str tt;
	for (int i = 0; i < data.size(); i++)
	{
		char c = data[i];
		if (c == interval && tt.size()>0)
		{
			re += tt;
			tt = "";
		}
		else
		{
			tt += c;
		}
	}
	return re;
}

arr<str> str::pieces(const arr<int> intervals) const
{
	arr<str> re;
	if (intervals.size() == 0)
	{
		re += data;
		return re;
	}
	str tt;
	int i1 = 0;
	for (int i = 0; i < data.size(); i++)
	{
		char c = data[i];
		if (i1 < intervals.size() && i == intervals[i1])
		{
			re += tt;
			i1 += 1;
			tt = "";
		}
		else
		{
			tt += c;
		}
	}
	if (tt.size() > 0)
	{
		re += tt;
	}
	return re;
}
//### str

//### global str
ostream& operator <<(ostream & os, const str & s)
{
	return os << s.data;
}

str operator+(const char* s, const str other)
{
	str re = s;
	re += other;
	return re;
}
//### global str

//### int2
int2::int2():x(0),y(0)
{

}

int2::int2(int x_, int y_):x(x_),y(y_)
{

}

int2 int2::operator+(const int2& t)
{
	return int2(x+t.x, y+t.y);
}

str int2::ToStr()
{
	str  re;
	re += x;
	re += " ";
	re += y;
	return re;
}
//### int2