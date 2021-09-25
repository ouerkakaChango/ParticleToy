#pragma once

#include "InterpUtility.h"
#include "Taiji.h"

namespace Interp
{
	class Interpreter
	{
		THISY(Interpreter)
		Interpreter(LanguageType srcType_, LanguageType dstType_);

		LanguageType srcType, dstType;
	};
}