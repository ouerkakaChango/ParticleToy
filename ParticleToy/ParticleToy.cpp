// ParticleToy.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "FileHelper/FileUtility.h"
#include "Interpreter/Interpreter.h"

using std::cout;
using std::endl;

//describe思路一:
//describe分部分:context,define,use condition
//1.在转成目标代码时，context直接对应代码段
//2.define作为下级condition保留
//3.在遇到use时候，查找function对应的describe，并将condition嵌入，然后parse
//4.迭代parse子describe
//（最后所有逻辑都会转成某一级的context，然后正确转成目标代码）
using namespace Interp;
int main()
{
	PathGod.AutoConfig();
	PathGod.pathMap.add("InterpRes", PathGod.mainPath + "/include/Interpreter/Res");
	PathGod.pathMap.add("PexWorkPath", PathGod.mainPath + "/PythonWorkSpace/pex");
	
	Interpreter interp(PathGod.pathMap["PexWorkPath"] + "/testRT.pex", PathGod.pathMap["PexWorkPath"] + "/testRT_INTERP.py");
	interp.Interp();

	system("pause");
	return 0;
}