// ParticleToy.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>

#include "Spaces/Grid.h"

#include "FileHelper/StaticPointWriter.h"

//### 应用
//ParticleToy第五期应用：FFT海面

//https://www.slideshare.net/Codemotion/an-introduction-to-realistic-ocean-rendering-through-fft-fabio-suriano-codemotion-rome-2017
//--1.可视化 Phillips spectrum
//--(放弃使用自己的rayTraceRender。输出txt,到Houdini里去可视化。写框架将序列化代码通用化)
//2.迭代次数改1，照理来说1次迭代本质是sin函数，查一下错误。

int main()
{
	str pyWorkPath = "C:\\Users\\hasee\\source\\repos\\ParticleToy\\PythonWorkSpace";

	Grid oceanGrid;
	double L = 1024;
	double N = 24;
	P2 edge(N, N);
	oceanGrid.SetGridSettings<double>(edge.x, edge.y, L/N, 0.0);
	auto& grid = Cast<GridI<double>*>(oceanGrid.i[0])->grid;

	//###
	StaticPointWriter writer;
	writer.SetWriteMode(WriteMode_Houdini);

	double g = 9.8;
	P2 windDir(1, 1);
	windDir = safeNorm(windDir);
	double waveAmplitude= 5.0;
	double v = 100.0;
	double waveL = v * v / g;

	auto func_Ph = [&](P2 kvec)
	{
		double k = len(kvec);
		kvec = safeNorm(kvec);
		if (zero(k))
		{//!!! 处理PPT中未提到的除0情况
			return 0.0;
		}
		else
		{
			
			return waveAmplitude * exp(-1 / pow(k*waveL, 2)) / pow(k, 4) * pow(dot(kvec, windDir), 2);
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

		double height = 0.0;
		//???
		double t = 0.5;
		for (int j = 0; j < grid.datas.y; j++)
		{
			for (int i = 0; i < grid.datas.x; i++)
			{
				P2 kvec = grid.pnts[i][j] / (L/2) * 2 * PI;

				double wk = sqrt(g*len(kvec));

				cplx h_val = func_h0(func_Ph(kvec)) * e_cplx(wk*t) + conju(func_h0(func_Ph(-kvec)))* e_cplx(-wk * t);

				cplx final = h_val * e_cplx(dot(kvec, pos2d));

				static double lam = 3.0;
				double tH = final.real;// +lam * final.img * safeNorm(kvec).x + lam * final.img * safeNorm(kvec).y;

				height += tH;
			}
		}

		pointHeight = height;

		writer.addPoint(P(pos2d.x, pos2d.y, height));
	};

	grid.DoByPos(func);

	writer.Write("C:/HoudiniProjects/PToyScene/FFT.txt");

	return 0;
}