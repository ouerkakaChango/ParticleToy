// ParticleToy.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "FileHelper/FileUtility.h"
//#include "Interpreter/Interpreter.h"

using std::cout;
using std::endl;
//using namespace Interp;

//1.
//--- deprecated: 中间代码.dcb适用于任何2种语言，但实现起来太复杂，现在只需要pex->?
//--- describe思路一:
//--- describe分部分:context,define,use condition
//--- 1.在转成目标代码时，context直接对应代码段
//--- 2.define作为下级condition保留
//--- 3.在遇到use时候，查找function对应的describe，并将condition嵌入，然后parse
//--- 4.迭代parse子describe
//--- （最后所有逻辑都会转成某一级的context，然后正确转成目标代码）
//不得行

//2.
//--- deprecated: PexInterp结构，完全独立于Interp，专门解决pex->?
//要做的东西太多，而且这样.pex的功能基本类似.compute，索性先试一下unity.computeShader

int main()
{
	PathGod.AutoConfig();
	//PathGod.pathMap.add("InterpRes", PathGod.mainPath + "/include/Interpreter/Res");
	PathGod.pathMap.add("PexWorkPath", PathGod.mainPath + "/PythonWorkSpace/pex");
	
	//Interpreter interp(PathGod.pathMap["PexWorkPath"] + "/testRT.pex", PathGod.pathMap["PexWorkPath"] + "/testRT_INTERP.py");
	//interp.Interp();

	system("pause");
	return 0;
}