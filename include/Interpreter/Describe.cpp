#include "Describe.h"

#include <iostream>
#include <fstream>

namespace Interp
{
	void OutputDescribe(const str& describePath,
		const arr<str>& context,
		const map3<str, arr<str>>& toa)
	{
		std::cout << "Writing File...\n";
		std::ofstream f(describePath.data, std::ios::out);
		f.close();
		int aa = 1;
	}
}