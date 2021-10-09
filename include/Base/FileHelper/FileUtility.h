#pragma once

#include "Container.h"

void ReadFileToArr(const str& path, arr<str>& lines);
void ArrToFile(const arr<str>& lines, const str& path);

class PathGodClass
{
private:
	PathGodClass() {};
public:
	static PathGodClass& GetInstance();
	str GetCurrentPath();
	void AutoConfig();

	str mainPath;
	map<str, str> pathMap;
};
#define PathGod PathGodClass::GetInstance()