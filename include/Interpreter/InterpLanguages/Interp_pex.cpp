#include "Interp_pex.h"

#include "FileHelper/ReaderUtility.h"

#include <iostream>
using std::cout;
using std::endl;

namespace Interp
{
	void InterpreterO_pex::Load(const str& path)
	{
		cout << "PEX: load at "<< path << endl;
		arr<str> lines;
		ReadFileToArr(path, lines);

		ParseToDescribe(lines);
	}

	void InterpreterO_pex::ParseToDescribe(const arr<str>& lines)
	{
		for (int i=0;i<lines.size();i++)
		{
			str line = lines[i];
			//### parse
			if (IsIncludeFile(line))
			{
				
			}
			else if (IsObjDefine(line))
			{
				
			}
			else if (IsObjUse(line))
			{
				
			}
			else
			{
				//pex synax error
				abort();
			}
		}
	}

	bool InterpreterO_pex::validEnd(const str& line)
	{
		int inx = line.lastInxOf(';');
		for (int i = inx + 1; i < line.size(); i++)
		{
			if (line[i] != ' ')
			{
				return false;
			}
		}
		return true;
	}

	bool InterpreterO_pex::IsIncludeFile(str line)
	{
		line = line.NiceSpacebar();
		if (line.beginWith("#include "))
		{
			str file = line.clip("#include ");
			str contextName;
			if (ValidFile(file, contextName))
			{
				contextArr += contextName;
				return true;
			}
		}
		return false;
	}

	bool InterpreterO_pex::IsObjDefine(str line)
	{
		if (!validEnd(line))
		{
			return false;
		}
		line = line.NiceSpacebar();
		line = line.clipBack(';');
		//"type name(param1,...)"
		//1.Check "type[spacebar]" format
		str typeName, objName;
		arr<str> params;
		int inx1 = line.inxOf(' ');
		if (inx1 != -1)
		{
			typeName = line.clip(0, inx1);
			str objStr = line.clip(inx1+1);
			if (IsFunctionFormat(objStr, objName, params))
			{
				toa += typeName;
				toa[typeName] += objName;
				toa[typeName][objName].add("define", params);
				return true;
			}
		}
		return false;
	}

	bool InterpreterO_pex::IsObjUse(const str& line)
	{

		return true;
	}

	bool InterpreterO_pex::IsFunctionFormat(str s, str& funcName, arr<str>& params)
	{
		//funcName(p1,p2...)
		int inx1 = s.inxOf('(');
		str f = s.clip(0, inx1);
		if (f.isAllCharacter())
		{
			str right = s.clip(inx1);
			if (right.last() == ')')
			{
				funcName = f;
				right = right.range(1, right.size() - 2);
				params = GetParams(right);
				return true;
			}
		}

		return false;
	}
}