#include "Container.h"

//### str
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
//### str

//### global str
ostream& operator <<(ostream & os, const str & s)
{
	return os << s.data;
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