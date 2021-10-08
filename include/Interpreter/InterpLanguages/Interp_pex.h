#pragma once

#include "../Interpreter.h"
#include "../Describe.h"
namespace Interp
{


	class InterpreterO_pex : public InterpreterO
	{
	public:
		void Load(const str& path) override;

		arr<str> contextArr;
		map3<str, arr<str>> toa;//<typeName ObjName ActionName,params>
	protected:
		void ParseToDescribe(const arr<str>& lines);

		//---parse functions
		bool validEnd(const str& line);
		bool IsIncludeFile(str line);
		bool IsObjDefine(str line);
		bool IsObjUse(str line);
		bool IsFunctionFormat(str s, str& funcName, arr<str>& params);
		//___parse functions
	};
}