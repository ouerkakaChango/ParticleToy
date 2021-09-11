// ParticleToy.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
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
	//#######################################
	// ::show sphere 

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
	// ::show PerlinNoise 3D

	//double surface = 0.2;
	//Grid* bboxGrid = new Grid;
	//bboxGrid->SetGrid3DSettings<double>(20,20,20, 1.0, 0.0);
	//auto& grid = Cast<Grid3DI<double>*>(bboxGrid->i[0])->grid;
	//PerlinNoise pNoise(&grid,P(4,4,4)); 
	//
	//MarchingCube mcube_algo;
	//mcube_algo.SetSurface(surface);
	//mcube_algo.March(grid);
	//
	//StaticTriWriter triWriter;
	//triWriter.SetWriteMode(WriteMode_Houdini);
	//triWriter.Load(&mcube_algo.triArr);
	//triWriter.Write("D:/HoudiniProj/PToyScene/MCube_pnoise.txt");
	//
	//StaticPointWriter pWriter;
	//pWriter.SetWriteMode(WriteMode_Houdini);
	//GRID_PosFunc(f_writePoints, double)
	//{
	//	if (data >= surface)
	//	{
	//		pWriter.addPoint(pntPos);
	//	}
	//};
	//grid.DoByPos(f_writePoints);
	//pWriter.Write("D:/HoudiniProj/PToyScene/MCube_pnoisePnts.txt");

	//####################################### 
	//::show fbm
	auto random = [](P st) {
		return frac(sin(dot(st, P(0.63,0.76,0.476)))*
			43758.5453123);
	};

	// Based on Morgan McGuire @morgan3d
	// https://www.shadertoy.com/view/4dS3Wd
	auto noise = [&](P st) {
		P i = floor(st);
		P f = frac(st);

		// Four corners in 3D cell
		float c1 = random(i);
		float c2 = random(i + P(1, 0, 0));
		float c3 = random(i + P(1, 1, 0));
		float c4 = random(i + P(0, 1, 0));
		float c5 = random(i + P(0, 0, 1));
		float c6 = random(i + P(1, 0, 1));
		float c7 = random(i + P(1, 1, 1));
		float c8 = random(i + P(0, 1, 1));

		P uvw = f * f * (3.0 - 2.0 * f);

		float lerpx1 = lerp(c1, c2, uvw.x);
		float lerpx2 = lerp(c4, c3, uvw.x);
		float lerpy1 = lerp(lerpx1, lerpx2, uvw.y);

		float lerpx3 = lerp(c5, c6, uvw.x);
		float lerpx4 = lerp(c8, c7, uvw.x);
		float lerpy2 = lerp(lerpx3, lerpx4, uvw.y);

		float lerpz = lerp(lerpy1, lerpy2, uvw.z);

		return lerpz;
	};

	double maxHeight = 0.5;

	auto fbm=[&](P st) {
		// Initial values
		float value = 0.0;
		float amplitude = maxHeight;
		float frequency = 0.;
		//
		// Loop of octaves
		for (int i = 0; i < 6; i++) {
			value += amplitude * noise(st);
			st *= 2.;
			amplitude *= .5;
		}
		return value;
	};

	Grid * bboxGrid = new Grid;
	double divide = 40;
	double edgelen = 1.0 / divide;
	double Zdivide = 20;
	bboxGrid->SetGrid3DSettings<double>(divide, divide, Zdivide, P(edgelen, edgelen,1.0/ Zdivide), 0.0);
	auto& grid3d = Cast<Grid3DI<double>*>(bboxGrid->i[0])->grid;

	GRID_IDPosFunc(f_initWeights, double)
	{
		const P& pos = pntPos;
		if (k == 0)
		{

			data = 1 + fbm(P(pos.x, pos.y, 0.0));
		}
		else
		{
			double height = grid3d.datas[i][j][0] - 1;
			data = pos.z < height ? 1 : 0;
		}
	};
	grid3d.DoByIDPos(f_initWeights);

	double surface = 0.5;
	MarchingCube mcube_algo;
	mcube_algo.SetSurface(surface);
	mcube_algo.March(grid3d);
	
	StaticTriWriter triWriter;
	triWriter.SetWriteMode(WriteMode_Houdini);
	triWriter.Load(&mcube_algo.triArr);
	triWriter.Write("D:/HoudiniProj/PToyScene/MCube_terrain.txt");

	StaticPointWriter pWriter;
	pWriter.SetWriteMode(WriteMode_Houdini);
	GRID_PosFunc(f_writePoints, double)
	{
		if (data >= surface)
		{
			pWriter.addPoint(pntPos);
		}
	};
	grid3d.DoByPos(f_writePoints);
	pWriter.Write("D:/HoudiniProj/PToyScene/MCube_terrainPnts.txt");

	//####################################### 
	//::show all Marching Cube pieces

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