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

#define PEX LanguageType_pex
#define PYTHON LanguageType_python
}