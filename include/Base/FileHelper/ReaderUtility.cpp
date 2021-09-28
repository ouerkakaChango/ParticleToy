#include "ReaderUtility.h"

#include <fstream>

void ReadFileToArr(const str& path, arr<str>& lines)
{
	std::ifstream f(path.data);
	while (!f.is_open())
	{
		f.open(path.data);
	}

	while (!f.eof())
	{
		str line;
		getline(f, line.data);
		lines += line;
	}
}