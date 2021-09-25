// ParticleToy.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#ifdef OLD_FFTOCEAN
#include "pch.h"
#include <iostream>

#include "Render/Material.h"
#include "rayTraceRender/rayTraceWorld.h"
#include "rayTraceRender/rayTraceScreen.h"
#include "Spaces/Grid.h"

//### 应用
//ParticleToy第五期应用：FFT海面

//https://www.slideshare.net/Codemotion/an-introduction-to-realistic-ocean-rendering-through-fft-fabio-suriano-codemotion-rome-2017
//1.可视化 Phillips spectrum

//extra:优化
//1.为了优化大量静态物件的sdf过程，以可视化静态Phillips spectrum为例：
//确定好grid的长宽后，默认Box为一个平均高度h，然后将整个BounceSpace grid化。
//每一个grid存储的是sdf-min的ObjInx。
//在以后每次sdf时候，如果最近的两个ObjInx是一样的，那么直接O(1)找到min-sdf，而不是O(n)for循环去找minSDF。
//如果不一样，那么两者比较取最小。
//如果grid分辨率合理，是能没有瑕疵的。细分太大显然会出问题。

int main()
{
	str pyWorkPath = "C:\\Users\\hasee\\source\\repos\\ParticleToy\\PythonWorkSpace";
	bool pbrMode = false;
	rayTraceWorld* world = new rayTraceWorld;
	world->SetTraceSettings(1, rayTraceMode_SDFSphere, rayTraceBounceMode_cheap, rayTraceMaterialMode_BlinnPhong);
	{
		//world->SetOptimizeMode(rayTraceOptimizeMode_PerTask);
		//auto opt = Cast<rayTraceOptimizePolicy_PerTask*>(world->optimizePolicy);
		//opt->rayPerTask = 1080*6;
	}
	rayTraceWorldR* op = (rayTraceWorldR*)world->r[0];

	Grid oceanGrid;
	P2 edge(8, 5);
	oceanGrid.SetGridSettings<Box*>(edge.x, edge.y, 0.5, nullptr);
	auto& grid = Cast<GridI<Box*>*>(oceanGrid.i[0])->grid;

	//###
	double g = 9.8;
	P2 windDir(1, 1);
	windDir = safeNorm(windDir);
	P2 L = edge * 0.5;
	double waveAmplitude=0.3;

	auto func_Ph = [&](P2 kvec)
	{
		double k = len(kvec);
		return waveAmplitude * exp(-1 / pow(k*len(L), 2)) / pow(k, 4) * pow(dot(kvec, windDir), 2);
	};

	auto func_h0 = [&](double Ph)
	{
		double ksi1 = rand_norm(0, 1);
		double ksi2 = rand_norm(0, 1);
		return 1 / sqrt(2) * cplx(ksi1, ksi2) * sqrt(Ph);
	};

	auto func = [&](Box*& box, P2 pos2d)
	{
		P2 kvec = pos2d / L;
		double k = len(kvec);
		//???
		double t = 0.0;

		double wk = sqrt(g*k);

		cplx h0_val = func_h0(func_Ph(kvec));

		cplx h_val = h0_val * e_cplx(wk*t) + conju(func_h0(func_Ph(-kvec)))* e_cplx(-wk*t);

		double height = 1 + h_val.real;

		if (height < 0)
		{
			std::cout << height << " ";
		}

		box = new Box(P(pos2d.x, -2.5, pos2d.y), P(0.2, height, 0.2));
		box->center.y += height/2;
		op->PutShape(box, "oceanSpectum");
	};
	grid.DoByPos(func);
	
	auto screen = new rayTraceScreen(1080,720);
	screen->Translate(P(0, 0, 7));
	op->PutScreen(screen);
	
	auto light = new DirectionalLight(P(-1, -1, 0), P(1, 1, 1));
	op->PutLight(light);

	op->Evolve();

	op->SaveScreenBufferFrame(screen,"color", pyWorkPath+"\\z.txt");
	_chdir(pyWorkPath.data.c_str());
	system("ZZZRUN_TransToImg.bat");
	//op->SayI();
	return 0;
}
#endif