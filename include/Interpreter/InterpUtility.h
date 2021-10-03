#pragma once
#include "Container.h"

namespace Interp
{

	enum LanguageType
	{
		LanguageType_unknown,
		LanguageType_pex,
		LanguageType_python,
	};

	bool CheckType(const str& path, LanguageType type);
	LanguageType GetLanguageType(const str& path);
	bool ValidFile(str fileStr,str& fileName);
	arr<int2> Level1Brackets(const str& s);
	arr<int> FilterRanges(const arr<int>& intervals, const arr<int2>& ranges);
	arr<str> GetParams(const str& s);

#define PEX LanguageType_pex
#define PYTHON LanguageType_python
}