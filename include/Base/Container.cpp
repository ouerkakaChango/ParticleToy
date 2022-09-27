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

vector<size_t> str::allIndices(const str& sub)
{
	vector<size_t> positions; // holds all the positions that sub occurs within str

	size_t pos = data.find(sub.data, 0);
	while (pos != string::npos)
	{
		positions.push_back(pos);
		pos = data.find(sub.data, pos + 1);
	}

	return positions;
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

int str::inxOf(const char& c, int begin) const
{
	for (int i = begin; i < data.size(); i++)
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
	if (data.size() == 0)
	{
		return *this;
	}

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
	if (re.size()>=1 && re[re.size() - 1] == ' ')
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
	if (tt.size() > 0)
	{
		re += tt;
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

int str::tailInxOf(const str& s) const
{
	if (s.size() <= 1)
	{
		return inxOf(s[0]);
	}

	arr<int> indices = indicesOf(s[0]);
	int size = s.size();
	for (int i = 0; i < indices.size(); i++)
	{
		int inx1 = indices[i];
		if (range(inx1, inx1 + size - 1) == s)
		{
			return inx1 + size - 1;
		}
	}
	return -1;
}

str str::replace(int pos, int len, str newSub)
{
	return data.replace(pos, len, newSub.data);
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

str GetFileNameFromPath(const str& path)
{
	int inx1 = path.lastInxOf('.');
	int start = -1;
	for (int i = inx1; i >= 0; i--)
	{
		char c = path[i];
		if (c == '\\' || c == '/')
		{
			start = i;
			break;
		}
	}
	return path.range(start+1, inx1-1);
}

void GetFileNameFromPath(const str& path, str& folder, str&fName, str&postfix)
{
	int inx1 = path.lastInxOf('.');
	int start = -1;
	for (int i = inx1; i >= 0; i--)
	{
		char c = path[i];
		if (c == '\\' || c == '/')
		{
			start = i;
			break;
		}
	}
	fName = path.range(start + 1, inx1 - 1);
	folder = path.range(0, start);
	postfix = path.range(inx1, path.size() - 1);
	int aa = 1;
}

void NiceSpaceBar(arr<str>& lines)
{
	for (int i = 0; i < lines.size(); i++)
	{
		lines[i] = lines[i].NiceSpacebar();
	}
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