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

//https://zhuanlan.zhihu.com/p/64414956
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
	oceanGrid.SetGridSettings<Box*>(5, 5, 0.5, nullptr);
	auto& grid = Cast<GridI<Box*>*>(oceanGrid.i[0])->grid;

	//###
	P2 windDir(1, 1);

	auto func = [&](Box*& box, P2 pos2d)
	{
		double height = pow(dot(pos2d/2.5, windDir),2);
		box = new Box(P(pos2d.x, -1.0, pos2d.y), P(0.2, height, 0.2));
		box->center.y += height/2;
		op->PutShape(box, "oceanSpectum");
	};
	grid.DoByPos(func);
	
	auto screen = new rayTraceScreen(1080,720);
	screen->Translate(P(0, 0, 5));
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