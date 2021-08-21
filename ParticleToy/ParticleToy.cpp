// ParticleToy.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>

#include "Spaces/Grid.h"

using std::cout;
using std::endl;

//### 应用
//Marching Cube 算法原型测试
//https://www.bilibili.com/video/BV1Ta4y1E7CT?from=search&seid=16731839180621170656

//1.先把一个实心球的grid3D的权重表示出来。

//### 启发
//1.FileWrite的阴阳以前写反了，改了互换，发现不影响以前的用户代码，这个阴阳结构好处就出来了。

int main()
{
	//auto weightFunc = wf_solidSphere;
	Grid* bboxGrid = new Grid;
	bboxGrid->SetGrid3DSettings<float>(10, 10, 10, 0.4f, 0.0);
	auto& grid3d = Cast<Grid3DI<float>*>(bboxGrid->i[0])->grid;
	auto func = [](float& data, P pntPos, int pntInx)
	{
		cout << pntPos.ToStr() << endl;
	};
	grid3d.DoByPos(func);
	return 0;
}