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

	bool ValidFile(str fileStr, str& fileName)
	{
		int len = fileStr.size();
		if (fileStr[0] == '<' && fileStr[len - 1] == '>')
		{
			fileName = fileStr.range(1, len - 2);
			return true;
		}
		return false;
	}
}