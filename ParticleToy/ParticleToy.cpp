// ParticleToy.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <fstream>
#include "FileHelper/FileUtility.h"

using std::cout;
using std::endl;

int main()
{
	cout << "AutoCS";
	std::cout << "Writing File...\n";
	std::ofstream f("testoutFile.txt", std::ios::out);
	f.close();
	system("pause");
	return 0;
}