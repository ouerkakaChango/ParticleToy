#pragma once

#include "../Interpreter.h"

namespace Interp
{
	class InterpreterO_pex : public InterpreterO
	{
	public:
		void Load(const str& path) override;

	protected:
		void ParseToDescribe(const arr<str>& lines);

		//---parse functions
		bool IsIncludeFile(const str& line);
		bool IsObjDefine(const str& line);
		bool IsObjUse(const str& line);
		//___parse functions
	};
}