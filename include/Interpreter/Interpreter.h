#pragma once

#include "InterpUtility.h"
#include "Taiji.h"

namespace Interp
{
	class Interpreter
	{
		THISY(Interpreter)
		Interpreter(LanguageType srcType_, LanguageType dstType_);
		Interpreter(const str& srcPath_, const str& dstPath_);

		bool LoadSrc();
		void Interp();
		bool CheckTypeValid();

		LanguageType srcType, dstType;
		str srcPath, dstPath;
	};
}