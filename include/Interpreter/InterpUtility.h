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

#define PEX LanguageType_pex
#define PYTHON LanguageType_python
}