#include "InterpUtility.h"

namespace Interp
{
	bool CheckType(const str& path, LanguageType type)
	{
		if (type == LanguageType_pex)
		{
			return path.endWith(".pex");
		}
		else if (type == LanguageType_python)
		{
			return path.endWith(".py");
		}
		else
		{
			return false;
		}
	}

	LanguageType GetLanguageType(const str& path)
	{
		if (path.endWith(".pex"))
		{
			return LanguageType_pex;
		}
		else if (path.endWith(".py"))
		{
			return LanguageType_python;
		}
	}
}