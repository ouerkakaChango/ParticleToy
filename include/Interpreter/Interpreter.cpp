#include "Interpreter.h"

#include "InterpLanguageHeader.h"
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
		auto to = Cast<InterpreterO*>(o[0]);
		to->Load(srcPath);

		auto ti = Cast<InterpreterI*>(i[0]);
		//???
		ti->Parse("C:/Personal/ParticleToy/PythonWorkSpace/pex/test_RTSphere.describe");
		return true;
	}

	void Interpreter::Interp()
	{
		if (!CheckTypeValid())
		{
			abort();
		}
		cout << "Interping..."<<endl;
		Init();
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

	void Interpreter::Init()
	{
		if (srcType == PEX)
		{
			auto to = new InterpreterO_pex;
			o += to;
		}
		if (dstType == PYTHON)
		{
			auto ti = new InterpreterI_python;
			i += ti;
		}
	}

}