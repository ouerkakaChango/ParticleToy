#pragma once

#include "../Interpreter.h"

namespace Interp
{
	class InterpreterO_pex : public InterpreterO
	{
	public:
		void Load(const str& path) override;

		arr<str> contextArr;
		map3<str,str> toa;//typeObjAction
	protected:
		void ParseToDescribe(const arr<str>& lines);

		//---parse functions
		bool validEnd(const str& line);
		bool IsIncludeFile(str line);
		bool IsObjDefine(str line);
		bool IsObjUse(const str& line);
		//___parse functions
	};
}