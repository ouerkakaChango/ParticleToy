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

	double L = 1024.0;
	double N = 512.0;
	Grid oceanGrid;
	oceanGrid.SetGridSettings<double>(N, N, L/N, 0.0);
	auto& grid = Cast<GridI<double>*>(oceanGrid.i[0])->grid;

	//###
	StaticPointWriter writer;
	writer.SetWriteMode(WriteMode_Houdini);

	double g = 9.8;
	P2 windDir(0.8, 0.3);
	windDir = safeNorm(windDir);
	double waveAmplitude= 25.0; 
	double v = 10.0;
	double waveL = v * v / g;

	auto func_Ph = [&](P2 kvec)
	{
		//??? debug
		return waveAmplitude;

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
		//??? debug
		//return e_cplx(0.0);

		//!!! 对于同一个频率kvec，得是同一组ksi1,ksi2
		double ksi1 = rand_norm(0, 1);
		double ksi2 = rand_norm(0, 1);

		return 1 / sqrt(2) * cplx(ksi1, ksi2) * sqrt(Ph);
	};

	auto func = [&](double& pointHeight, P2 pos2d)
	{
		int count = 0;
		int MAXCOUNT = 1;

		double height = 0.0;
		//???
		double t = 0.5;
		for (int j = 0; j < grid.datas.y; j++)
		{
			for (int i = 0; i < grid.datas.x; i++)
			{
				//??? debug
				//P2 kvec = grid.pnts[i][j] / (L/2) * 2 * PI;
				P2 kvec = grid.pnts[N / 2 + 1][N / 2] / (L / 2) * 2 * PI;
				//P2 kvec = grid.pnts[0][0] / (L / 2) * 2 * PI;

				double wk = sqrt(g*len(kvec));

				//此时它的img已经为0
				cplx h0 = func_h0(func_Ph(kvec));
				cplx h_val = h0 * e_cplx(wk*t) + conju(h0)* e_cplx(-wk * t);

				//??? debug
				cplx final = h_val * e_cplx(dot(kvec, pos2d));
				//cplx final = e_cplx(dot(P2(0.1,0.1), pos2d));

				//???
				static double lam = 0.0;
				double tH = final.real;// +lam * final.img * safeNorm(kvec).x + lam * final.img * safeNorm(kvec).y;

				height += tH;

				//??? debug
				count++;
				if (count == MAXCOUNT)
				{
					break;
				}
			}
			if (count == MAXCOUNT)
			{
				break;
			}
		}

		pointHeight = height + waveAmplitude;

		writer.addPoint(P(pos2d.x, pos2d.y, height));
	};

	grid.DoByPos(func);

	writer.Write("C:/HoudiniProjects/PToyScene/FFT.txt");

	return 0;
}