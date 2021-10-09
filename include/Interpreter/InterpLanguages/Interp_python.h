#pragma once

#include "../Interpreter.h"

namespace Interp
{
	class InterpreterI_python : public InterpreterI
	{
	public:
		void Parse(const str& path) override;
		void PreHandle(); //¼ÓutfÍ·

		//--- Parse
		void ParseLine(str line);
		void ParseContext(const str& ctxName);
		void ParseDefine(str line);
		void ParseUse(str line);
		//___

		
		arr<str> lines;
		map3<str, arr<str>> toa;//<typeName ObjName ActionName,params>
	};
}