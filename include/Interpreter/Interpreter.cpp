#include "Interpreter.h"

namespace Interp
{
	Interpreter::Interpreter()
	{

	}

	Interpreter::Interpreter(LanguageType srcType_, LanguageType dstType_)
		:srcType(srcType_), dstType(dstType_)
	{

	}

	void Interpreter::LoadSrc(const str& srcPath)
	{
		if (!CheckType(srcPath, srcType))
		{
			return;
		}
	}

}