#include "InterpUtility.h"

namespace Interp
{
	bool CheckType(const str& path, LanguageType type)
	{
		if (type == LanguageType_pex)
		{
			return path.clipBack(0, 4) == ".pex";
		}
		else
		{
			return false;
		}
	}
}