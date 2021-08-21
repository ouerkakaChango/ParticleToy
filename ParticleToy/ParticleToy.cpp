// ParticleToy.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>

#include "Spaces/Grid.h"
#include "Discretization/MarchingCube/MarchingCube.h"

#include"FileHelper/StaticPointWriter.h"

using namespace Discretization;
using std::cout;
using std::endl;

//### 应用
//Marching Cube 算法原型测试
//https://www.bilibili.com/video/BV1Ta4y1E7CT?from=search&seid=16731839180621170656

//1.先把一个实心球的grid3D的权重表示出来。

//### 启发
//1.FileWrite的阴阳以前写反了，改了互换，发现不影响以前的用户代码，这个阴阳结构好处就出来了。

double wf_solidSphere(P pos)
{
	if (pos.len() <= 1)
	{
		return 1.0;
	}
	else
	{
		return 0.0;
	}
}

int main()
{
	StaticPointWriter pWriter;
	pWriter.SetWriteMode(WriteMode_Houdini);

	auto weightFunc = wf_solidSphere;
	Grid* bboxGrid = new Grid;
	//1000 个 cell
	bboxGrid->SetGrid3DSettings<double>(10, 10, 10, 0.4f, 0.0);
	auto& grid3d = Cast<Grid3DI<double>*>(bboxGrid->i[0])->grid;
	grid3d.Centerlize();
	GRID_PosFunc(f_initGrid, double)
	{
		data = weightFunc(pntPos);
		//cout << pntPos.ToStr() <<" "<< data<< endl;
		if (data > 0.5)
		{
			//pWriter.addPoint(pntPos);
		}
	};
	grid3d.DoByPos(f_initGrid);

	MarchingCube mcube_algo;
	mcube_algo.SetSurface(0.5);
	mcube_algo.March(grid3d);

	//pWriter.Write("D:/HoudiniProj/PToyScene/MCube_pnts.txt");
	return 0;
}