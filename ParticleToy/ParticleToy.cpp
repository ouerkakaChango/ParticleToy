// ParticleToy.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>

#include "Spaces/Grid.h"

#include "FileHelper/StaticPointWriter.h"

//### 应用
//ParticleToy第五期应用：FFT海面

//https://www.slideshare.net/Codemotion/an-introduction-to-realistic-ocean-rendering-through-fft-fabio-suriano-codemotion-rome-2017
//1.可视化 Phillips spectrum
//(放弃使用自己的rayTraceRender。输出txt,到Houdini里去可视化。写框架将序列化代码通用化)

int main()
{
	str pyWorkPath = "C:\\Users\\hasee\\source\\repos\\ParticleToy\\PythonWorkSpace";

	Grid oceanGrid;
	P2 edge(64, 64);
	oceanGrid.SetGridSettings<double>(edge.x, edge.y, 1.0, 0.0);
	auto& grid = Cast<GridI<double>*>(oceanGrid.i[0])->grid;

	//###
	StaticPointWriter writer;
	writer.SetWriteMode(WriteMode_Houdini);

	double g = 9.8;
	P2 windDir(1, 1);
	windDir = safeNorm(windDir);
	P2 L = edge * 0.5;
	double waveAmplitude=0.3;

	auto func_Ph = [&](P2 kvec)
	{
		double k = len(kvec);
		if (zero(k))
		{//!!! 处理PPT中未提到的除0情况
			return 0.0;
		}
		else
		{
			return waveAmplitude * exp(-1 / pow(k*len(L), 2)) / pow(k, 4) * pow(dot(kvec, windDir), 2);
		}
	};

	auto func_h0 = [&](double Ph)
	{
		double ksi1 = rand_norm(0, 1);
		double ksi2 = rand_norm(0, 1);
		return 1 / sqrt(2) * cplx(ksi1, ksi2) * sqrt(Ph);
	};

	auto func = [&](double& pointHeight, P2 pos2d)
	{
		//??? debug
		if (pos2d == P2(-1, -1))
		{
			int aa = 1;
		}

		P2 kvec = pos2d / L;
		double k = len(kvec);
		//???
		double t = 0.0;

		double wk = sqrt(g*k);

		cplx h0_val = func_h0(func_Ph(kvec));

		cplx h_val = h0_val * e_cplx(wk*t) + conju(func_h0(func_Ph(-kvec)))* e_cplx(-wk*t);

		double height = 1 + h_val.real;

		pointHeight = height;

		writer.addPoint(P(pos2d.x, pos2d.y, height));
	};

	grid.DoByPos(func);

	writer.Write("C:/HoudiniProjects/PToyScene/FFT.txt");

	return 0;
}