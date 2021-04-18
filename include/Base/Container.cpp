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

bool str::operator==(const char* s)
{
	return data == string(s);
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