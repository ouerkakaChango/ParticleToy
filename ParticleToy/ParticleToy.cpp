// ParticleToy.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>

#include "Render/Material.h"
#include "rayTraceRender/rayTraceWorld.h"
#include "rayTraceRender/rayTraceScreen.h"

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

	rayTraceWorldR* op = (rayTraceWorldR*)world->r[0];

	auto s1 = op->PutShape(new Sphere(P(0, 0, -5), 1.0),"Sphere1");
	if (!pbrMode)
	{
		auto param = Cast<BlinnPhongI*>(s1->material->i[0]);
		param->kS = 0.1;
		param->specPower = 1.0;
	}
	else
	{
		auto param = Cast<PBRI*>(s1->material->i[0]);
		//白/红色球
		//param->albedo = P(1, 0, 0);
		//param->emissive = P(0, 0, 1);
	}

	if (true)
	{
		auto box1 = op->PutShape(new Box(P(0.0, -1.2, -5.0), P(5.0, 0.1, 5.0)), "Box1");
		if (!pbrMode)
		{
			auto param = Cast<BlinnPhongI*>(box1->material->i[0]);
			param->kD = 0.05;
			param->kS = 1.0;
			param->specPower = 5.0;
			param->specularColor = P(1.0, 242 / 255.0, 0.0);
		}
		else
		{
			auto param = Cast<PBRI*>(box1->material->i[0]);
		}
	}
	
	auto screen = new rayTraceScreen(1080,720);
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