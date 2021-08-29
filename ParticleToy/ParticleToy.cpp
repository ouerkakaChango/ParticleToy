﻿// ParticleToy.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>

#include "Spaces/Grid.h"
#include "Discretization/MarchingCube/MarchingCube.h"

#include "FileHelper/StaticPointWriter.h"
#include "FileHelper/StaticTriWriter.h"

#include "Noise/PerlinNoise.h"

using namespace Discretization;
using std::cout;
using std::endl;

//### 应用
//Marching Cube 算法原型测试
//https://www.bilibili.com/video/BV1Ta4y1E7CT?from=search&seid=16731839180621170656

//1.先把一个实心球的grid3D的权重表示出来。
//2.实现3D perlin noise （30*30*30），然后输出tri

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
	//StaticPointWriter pWriter;
	//pWriter.SetWriteMode(WriteMode_Houdini);
	//
	//StaticTriWriter triWriter;
	//triWriter.SetWriteMode(WriteMode_Houdini);
	//
	//auto weightFunc = wf_solidSphere;
	//Grid* bboxGrid = new Grid;
	//{
	//	double divide = 40;
	//	double edgelen = 4.0 / divide;
	//	bboxGrid->SetGrid3DSettings<double>(divide, divide, divide, edgelen, 0.0);
	//	auto& grid3d = Cast<Grid3DI<double>*>(bboxGrid->i[0])->grid;
	//	grid3d.Centerlize();
	//	GRID_PosFunc(f_initGrid, double)
	//	{
	//		data = weightFunc(pntPos);
	//		//cout << pntPos.ToStr() <<" "<< data<< endl;
	//		if (data > 0.5)
	//		{
	//			pWriter.addPoint(pntPos);
	//		}
	//	};
	//	grid3d.DoByPos(f_initGrid);
	//
	//	MarchingCube mcube_algo;
	//	mcube_algo.SetSurface(0.5);
	//	mcube_algo.March(grid3d);
	//
	//	triWriter.Load(&mcube_algo.triArr);
	//	triWriter.Write("D:/HoudiniProj/PToyScene/MCube_tris.txt");
	//	pWriter.Write("D:/HoudiniProj/PToyScene/MCube_pnts.txt");
	//}

	//#######################################
	double surface = 0.2;
	Grid* bboxGrid = new Grid;
	bboxGrid->SetGrid3DSettings<double>(20,20,20, 1.0, 0.0);
	auto& grid = Cast<Grid3DI<double>*>(bboxGrid->i[0])->grid;
	PerlinNoise pNoise(&grid,P(4,4,4)); 
	
	MarchingCube mcube_algo;
	mcube_algo.SetSurface(surface);
	mcube_algo.March(grid);
	
	StaticTriWriter triWriter;
	triWriter.SetWriteMode(WriteMode_Houdini);
	triWriter.Load(&mcube_algo.triArr);
	triWriter.Write("D:/HoudiniProj/PToyScene/MCube_pnoise.txt");
	
	StaticPointWriter pWriter;
	pWriter.SetWriteMode(WriteMode_Houdini);
	GRID_PosFunc(f_writePoints, double)
	{
		if (data >= surface)
		{
			pWriter.addPoint(pntPos);
		}
	};
	grid.DoByPos(f_writePoints);
	pWriter.Write("D:/HoudiniProj/PToyScene/MCube_pnoisePnts.txt");

	//####################################### 
	
	//MarchingCube mcube_algo;
	//mcube_algo.SetSurface(0.5);
	//mcube_algo.TestAll();
	//
	//StaticTriWriter triWriter;
	//triWriter.SetWriteMode(WriteMode_Houdini);
	//triWriter.Load(&mcube_algo.triArr);
	//triWriter.Write("D:/HoudiniProj/PToyScene/MCube_test.txt");

	return 0;
}