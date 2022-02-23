#include "Debug.h"

#include <iostream>
using std::cout;
using std::endl;

void DebugLines(const arr<str>& lines)
{
	for (int i = 0; i < lines.size(); i++)
	{
		cout << lines[i] << endl;
	}
}