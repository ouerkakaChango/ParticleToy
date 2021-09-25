// ParticleToy.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "Interpreter/Interpreter.h"
using std::cout;
using std::endl;

using namespace Interp;
int main()
{
	Interpreter interp(PEX, PYTHON);
	return 0;
}