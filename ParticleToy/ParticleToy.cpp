// ParticleToy.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "FileHelper/FileUtility.h"
#include "Interpreter/Interpreter.h"
using std::cout;
using std::endl;

using namespace Interp;
int main()
{
	//PathGod.pathMap.add("InterpRes", "C:\\Personal\\ParticleToy\\include/Interpreter/Res");
	PathGod.pathMap.add("InterpRes", "C:\\Users\\hasee\\source\\repos\\ParticleToy\\include/Interpreter/Res");
	PathGod.pathMap.add("PexWorkPath", "C:\\Users\\hasee\\source\\repos\\ParticleToy\\PythonWorkSpace/pex/");

	//Interpreter interp(PEX, PYTHON);
	//interp.LoadSrc(pyWorkPath + "/pex/testRT.pex");

	Interpreter interp(PathGod.pathMap["PexWorkPath"] + "testRT.pex", PathGod.pathMap["PexWorkPath"] + "testRT_INTERP.py");
	interp.Interp();


	return 0;
}