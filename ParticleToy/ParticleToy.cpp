// ParticleToy.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>

#include "FFTOcean/OceanParticleInfo.h"

#include "Spaces/Grid.h"
#include "FileHelper/StaticPointWriter.h"

using std::cout;
using std::endl;

//### 应用
//ParticleToy第五期应用：FFT海面

//https://www.slideshare.net/Codemotion/an-introduction-to-realistic-ocean-rendering-through-fft-fabio-suriano-codemotion-rome-2017
//--1.可视化 Phillips spectrum
//--(放弃使用自己的rayTraceRender。输出txt,到Houdini里去可视化。写框架将序列化代码通用化)
//2.迭代次数改1，照理来说1次迭代本质是sin函数，查一下错误。

int main()
{
	str pyWorkPath = "C:\\Users\\hasee\\source\\repos\\ParticleToy\\PythonWorkSpace";

	double L = 32;
	double N = 64;
	Grid oceanGrid;
	oceanGrid.SetGridSettings<OceanParticleInfo>(N, N, L/N, 0.0);
	auto& grid = Cast<GridI<OceanParticleInfo>*>(oceanGrid.i[0])->grid;

	//###
	StaticPointWriter writer;
	writer.SetWriteMode(WriteMode_Houdini);

	double g = 9.8;
	P2 windDir(1, 0);
	windDir = safeNorm(windDir);
	//double waveAmplitude= 1/250.0; 
	//double v = 4.0;
	//double waveL = v*v/g;
	double waveAmplitude = 1.0/N;
	double waveL = 4.0;//1.32;//0.52;

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
			auto re = waveAmplitude * exp(-1 / pow(k*waveL, 2)) / pow(k, 4) * pow(dot(kvec, windDir), 2);
			return re;
		}
	};

	auto func_h0 = [&](const cplx& ksi,double Ph)
	{
		auto re = 0.7071 * ksi  * sqrt(Ph);
		return re;
	};

	auto func_InitKsi = [&](OceanParticleInfo& pntInfo, P2 pos2d)
	{
		pntInfo.ksi = cplx(rand_norm(0, 1), rand_norm(0, 1));
	};

	auto func = [&](OceanParticleInfo& pntInfo, P2 pos2d)
	{
		int count = 0;
		//??? debug
		int MAXCOUNT = 1;
		bool bBigWave = false;
		bool bDebug = false;

		double height = 0.0;
		//???
		double t = 0.5;
		for (int j = 0; j < grid.datas.y; j++)
		{
			for (int i = 0; i < grid.datas.x; i++)
			{
				P2 kvec = grid.pnts[i][j] / (L/2) * 2 * PI;

				//??? debug
				//&&count!=0
				if (bDebug && count == MAXCOUNT - 1 && (bBigWave ? true:count!=0))
				{
					kvec = grid.pnts[N / 2 + 5][N / 2] / (L / 2) * 2 * PI;
				}
				
				cplx ksi = grid.datas[i][j].ksi;
				if (bDebug &&count == MAXCOUNT - 1 && (bBigWave ? true : count != 0))
				{
					ksi = grid.datas[N / 2 + 5][N / 2].ksi;
				}

				double wk = sqrt(g*len(kvec));

				cplx h0 = func_h0(ksi, func_Ph(kvec));
				double h_val = 2 * (h0 * e_cplx(wk*t)).real;
				cplx final = h_val * e_cplx(dot(kvec, pos2d));

				//???
				static double lam = 0.0;
				double tH = final.real +lam * final.img * safeNorm(kvec).x + lam * final.img * safeNorm(kvec).y;

				height += tH;
				
				//??? debug
				count++;
				if (bDebug && count == MAXCOUNT)
				{
					break;
				}
			}
			if (bDebug && count == MAXCOUNT)
			{
				break;
			}
		}

		pntInfo.height = height;

		writer.addPoint(P(pos2d.x, pos2d.y, height));
	};

	grid.DoByPos(func_InitKsi);
	grid.DoByPos(func);

	writer.Write("C:/HoudiniProjects/PToyScene/FFT.txt");

	return 0;
}