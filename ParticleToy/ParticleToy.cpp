// ParticleToy.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <fstream>
#include "FileHelper/FileUtility.h"
#include "AutoCS/AutoCSCommonDef.h"
#include "Debug.h"

using std::cout;
using std::endl;

using namespace AutoCS;

int main(int argc, char* argv[])
{
	cout << "AutoCS"<<endl;
	std::ofstream log("AutoCS_Log.txt", std::ios::out);
	str taskFile;
	if (argc == 1)
	{
		log << "Error,No arg";
		taskFile = "C:/Personal/UnityRayTraceGitGee/TestRayTrace/Assets/Resources/SDFGameCS/Detail1_task.txt";
	}
	else
	{
		log << "Task File: " << argv[1];
		taskFile = argv[1];
	}
	arr<str> lines;
	FileToArr(taskFile, lines);
	//DebugLines(lines);
	arr<str> templates;
	arr<str> outs;
	arr<str> cfgs;
	
	ParseTask(lines, templates, outs, cfgs);
	for (int i = 0; i < templates.size(); i++)
	{
		AutoCSCfg cfg(cfgs[i]);
		cfg.Generate(templates[i], outs[i]);
	}

	DebugLines(templates);
	DebugLines(outs);
	DebugLines(cfgs);

	log.close();
	system("pause");
	return 0;
}