#include "Interpreter.h"

#include <iostream>

using std::cout;
using std::endl;

namespace Interp
{
	Interpreter::Interpreter()
	{

	}

	Interpreter::Interpreter(LanguageType srcType_, LanguageType dstType_)
		:srcType(srcType_), dstType(dstType_)
	{

	}

	Interpreter::Interpreter(const str& srcPath_, const str& dstPath_)
		:srcPath(srcPath_),dstPath(dstPath_)
	{
		srcType = GetLanguageType(srcPath);
		dstType = GetLanguageType(dstPath);
	}

	bool Interpreter::LoadSrc()
	{
		//???
		return true;
	}

	void Interpreter::Interp()
	{
		if (!CheckTypeValid())
		{
			abort();
		}
		cout << "Interping...";
		if (!LoadSrc())
		{
			return;
		}
	}

	bool Interpreter::CheckTypeValid()
	{
		return srcType != LanguageType_unknown && dstType != LanguageType_unknown &&
			CheckType(srcPath, srcType) && CheckType(dstPath, dstType);
	}

}