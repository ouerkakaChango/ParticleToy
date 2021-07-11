// ParticleToy.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

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
	P2 windDir(1, 1);
	windDir = safeNorm(windDir);
	P2 L = edge * 0.5;
	double waveAmplitude=0.3;
	auto func = [&](Box*& box, P2 pos2d)
	{
		P2 kvec = pos2d / L;
		double k = len(kvec);
		//double height = waveAmplitude * exp(-1 / pow(dot(kvec,L), 2)) / pow(k, 4) * pow(dot(kvec, windDir),2);
		double height = waveAmplitude * exp(-1 / pow(k*len(L), 2)) / pow(k, 4) * pow(dot(kvec, windDir), 2);
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