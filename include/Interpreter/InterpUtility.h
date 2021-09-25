#pragma once
#include "Container.h"

namespace Interp
{

	enum LanguageType
	{
		LanguageType_pex,
		LanguageType_python,
	};

	bool CheckType(const str& path, LanguageType type);

#define PEX LanguageType_pex
#define PYTHON LanguageType_python
}