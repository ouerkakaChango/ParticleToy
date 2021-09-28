// ParticleToy.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "Interpreter/Interpreter.h"
using std::cout;
using std::endl;

using namespace Interp;
int main()
{
	str pexWorkPath = "C:\\Users\\hasee\\source\\repos\\ParticleToy\\PythonWorkSpace/pex/";

	//Interpreter interp(PEX, PYTHON);
	//interp.LoadSrc(pyWorkPath + "/pex/testRT.pex");

	Interpreter interp(pexWorkPath + "testRT.pex", pexWorkPath + "testRT_INTERP.py");
	interp.Interp();
	return 0;
}