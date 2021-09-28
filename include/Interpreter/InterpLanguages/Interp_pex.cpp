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

	bool InterpreterO_pex::IsIncludeFile(const str& line)
	{
		//line.beginWith("#include")
		return true;
	}

	bool InterpreterO_pex::IsObjDefine(const str& line)
	{
		
		return true;
	}

	bool InterpreterO_pex::IsObjUse(const str& line)
	{

		return true;
	}

}