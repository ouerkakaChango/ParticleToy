#include "Interp_python.h"

#include "FileHelper/FileUtility.h"
#include <iostream>
using std::cout;
using std::endl;

namespace Interp
{
	void InterpreterI_python::Parse(const str& path)
	{
		cout << "Python parse at " << path;
		arr<str> dcbLines;
		ReadFileToArr(path, dcbLines);

		//???
		PreHandle();
		for (int i = 0; i < dcbLines.size(); i++)
		{
			ParseLine(dcbLines[i]);
		}

		//output
		str folder, fName, postfix;
		GetFileNameFromPath(path, folder, fName, postfix);
		ArrToFile(lines, folder+fName+"_AUTO.py");
		int aa = 1;
	}

	void InterpreterI_python::PreHandle()
	{
		lines += "# -*- coding: utf-8 -*- ";
	}

	void InterpreterI_python::ParseLine(str line)
	{
		line = line.NiceSpacebar();
		if (line.beginWith("context "))
		{
			str context = line.clip("context ");
			ParseContext(context);
		}
		else if (line.beginWith("define "))
		{
			ParseDefine(line);
		}
		else if (line.beginWith("use "))
		{
			ParseUse(line);
		}
		else
		{
			abort();
		}
	}

	void InterpreterI_python::ParseContext(const str& ctxName)
	{
		if (ctxName == "RayTrace")
		{
			arr<str> tt;
			ReadFileToArr(PathGod.pathMap["InterpRes"] + "/py_ctx_" + ctxName +".py", tt);
			lines += tt;
		}
	}

	void InterpreterI_python::ParseDefine(str line)
	{//define Sphere called 's', only use 2,4 word
		str defStr = line.range(0, line.inxOf(',')-1);
		auto words = defStr.pieces(' ');
		str typeName = words[1];
		str objName = words[3].range(1, words[3].size() - 2);

		str paramStr = line.clip(line.inxOf(',') + 1);
		arr<str> params = GetParams(paramStr);
		for (int i = 0; i < params.size(); i++)
		{
			params[i] = params[i].NiceSpacebar();
		}
		toa[typeName][objName]["define"] = params;
 		int aa = 1;
	}

	void InterpreterI_python::ParseUse(str line)
	{
		str defStr = line.range(0, line.inxOf(',') - 1);
		auto words = defStr.pieces(' ');
		str funcName = words[3];
		str objName = words[1].range(1, words[1].size() - 2);

		str paramStr = line.clip(line.inxOf(',') + 1);
		arr<str> params = GetParams(paramStr);
		for (int i = 0; i < params.size(); i++)
		{
			params[i] = params[i].NiceSpacebar();
		}

		toa.Inx2(objName)[funcName] = params;
		//auto tt = toa.Inx2(objName)[funcName];
		//auto tt = toa["RT_screen"][objName][funcName];
		int aa = 1;
	}

}