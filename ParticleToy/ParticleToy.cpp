// ParticleToy.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>

#include "Render/Material.h"
#include "rayTraceRender/rayTraceWorld.h"
#include "rayTraceRender/rayTraceScreen.h"
#include "rayTraceRender/TraceMode/SDFSphereTrace/TraceRayI_SDFSphereMonteCarlo.h"

//### 架构心得
//1.基本的Space+Evolver+Solver一套，如同template,如同原点。

//### 应用
//ParticleToy第四期应用：光子渲染
//1.不完全沿用之前的世界，主要是Minkowsiki中的时间t,在渲染world中是不需要的
//2.放置光子发射器。(houdini坐标系，z向屏幕内)。光子发射器位置(0,0,-5)，朝(0,0,1)方向持续发射光子
//3.我认为在此应用中，光子与质点完全不同，它一瞬间反弹完所有路径，所以在光子渲染中，world的Evovle不依靠t，而是第一次bounce,
//第二次bounce...
//4.对world进行1次bounce的Evolve。
//5.放置“眼画布”，眼画布比相机还简单，不带任何投影矩阵，焦距等。
//而且特别大，光子打到它上面成为“画布上的一个像素点”（根据画布分辨率不同，事实上可能影响多个）。
//6.将眼画布上的颜色输出txt画布文件，外部python用pillow转画布文件为图片
//7.CUDA加速，首先是平行计算单光束和每个三角面判断相交

//8.为了渲染效果，改rayCast为rayTrace，改三角面相交为场景sdf sphere rayTrace。CUDA加速的是每条ray与场景的Intersect
//9.放置一个球体在(0,0,-5)，半径为1
//10.放置屏幕x属于[-1,1]，y根据分辨率变化的screen，eye在(0,1,0)对每个像素中心发射ray(trace束)
//11.将debugFrameBuffer的内容保存成png

//---rayTraceBounceMode_reflect:
//---对于3Bounce:
//---trace1:infos += info1, ray.color=Calcu(lightsInfo1,p1,v...)
//---trace2:infos += info2,
//---trace3:infos += info3,
//---gather1:indir3 = reflectK1 * Calcu(lightsInfo3,p3,norm(p2-p3),...)
//---gather2:indir2 = reflectK2 * ( Calcu(indir3,p2,norm(p1-p2),...) + Calcu(lightsInfo2,p2,norm(p1-p2),...) )
//---gather3:ray.color += Calcu(indir2,p1,v,...) 

//12.为了解决内存不够的问题：增加screen的traceMode。之前是screen保存所有ray,下一个bounce更新所有ray。
//增加screen的模式，能一个ray一个ray的Trace，写进buffer，然后释放此ray的内存，再进行下一次

//13.重要性采样。 根据：https://agraphicsguy.wordpress.com/2015/11/01/sampling-microfacet-brdf/ ,
//在CreateSubRays时候：
//13.1 均撒e,phi，e根据公式生成theta
//13.2 根据theta,phi用球坐标系生成向量h_local
//13.4 根据n法平面调整h_local为h_world
//13.5 令l = safeNorm(h-v)，如果l为0就重新撒。
//13.6 沿l方向生成subray
//13.7 在材质的pdf里根据cos(theta) = dot(h,n)的theta计算pdf，此theta和createSubRay的球坐标系theta是一样的。

int main()
{
	bool pbrMode = true;
	rayTraceWorld* world = new rayTraceWorld;
	if (pbrMode)
	{
		//world->SetTraceSettings(2, rayTraceMode_SDFSphere, rayTraceBounceMode_reflect, rayTraceMaterialMode_PBR);
		world->SetTraceSettings(3, rayTraceMode_SDFSphere, rayTraceBounceMode_MonteCarlo, rayTraceMaterialMode_PBR);

		//TraceRayI_SDFSphereMonteCarlo::sampleMode = rayTraceSampleMode_ImportanceSampling;
		TraceRayI_SDFSphereMonteCarlo::spp = 5;
		world->SetOptimizeMode(rayTraceOptimizeMode_PerTask);
		auto opt = Cast<rayTraceOptimizePolicy_PerTask*>(world->optimizePolicy);
		opt->rayPerTask = 540*6;
	}
	else
	{
		world->SetTraceSettings(1, rayTraceMode_SDFSphere, rayTraceBounceMode_cheap, rayTraceMaterialMode_BlinnPhong);
	}
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
		//param->albedo = P(0, 1, 0);
		//param->metallic = 0.7;
		//param->roughness = 0.3;
		//param->ambientRate = 0;
		//param->emissive = P(0.1, 0.0, 0.0);
		//param->albedo = P(1, 0, 0);
		//param->albedo = P(1.0, 242 / 255.0, 0.0);
	}
	//###
	//灯Box
	auto lightBox = op->PutShape(new Box(P(0.0, 4.0, -5.0), P(1.0, 0.1, 1.0)*0.8), "Box2");
	if (pbrMode)
	{
		auto param = Cast<PBRI*>(lightBox->material->i[0]);
		param->ambientRate = 0;
		param->emissive = P(5,5,5);
	}

	if(true)
	{
		double xDis = 2.0;
		//上
		auto box2 = op->PutShape(new Box(P(0.0, 4.05, -5.0), P(8.0, 0.1, 8.0)), "Box2");
		//左
		auto box3 = op->PutShape(new Box(P(-xDis, 0.0, -5.0), P(0.1, 5.0, 5.0)), "Box3");
		if (pbrMode)
		{
			auto param = Cast<PBRI*>(box3->material->i[0]);
			//左红
			param->albedo = P(1, 0, 0);
		}
		//右
		auto box4 = op->PutShape(new Box(P(xDis, 0.0, -5.0), P(0.1, 5.0, 5.0)), "Box4");
		if (pbrMode)
		{
			auto param = Cast<PBRI*>(box4->material->i[0]);
			//右绿
			param->albedo = P(0, 1, 0);
		}
		//后
		auto box5 = op->PutShape(new Box(P(0.0, 0.0, -6.5), P(8.0, 8.0, 0.1)), "Box5");
	}
	//###

	auto screen = new rayTraceScreen(1080,720);
	//auto screen = new rayTraceScreen(540, 360);
	//auto screen = new rayTraceScreen(270, 180);
	//screen->Translate(P(0.0, 0.0, -2.5));
	op->PutScreen(screen);
	//放完screen后，才给所有物体的材质加上了extra
	if (!pbrMode)
	{
		{
			//auto bounceParam = Cast<Extra_BlinnPhongI_CheapBounce*>(s1->material->i[1]);
			//bounceParam->reflectness = 0.5;
		}
		{
			//auto bounceParam = Cast<Extra_BlinnPhongI_CheapBounce*>(box1->material->i[1]);
			//bounceParam->reflectness = 0.5;
		}
	}
	else
	{
		if (world->bounceMode == rayTraceBounceMode_reflect)
		{
			{
				//auto bounceParam = Cast<Extra_ReflectBounce*>(s1->material->i[1]);
				//bounceParam->reflectEnegyRate = 1.0;
			}
			{
				//auto bounceParam = Cast<Extra_ReflectBounce*>(box1->material->i[1]);
				//bounceParam->reflectEnegyRate = .8;
			}
		}
		else if (world->bounceMode == rayTraceBounceMode_MonteCarlo)
		{
			//auto bounceParam = Cast<Extra_ReflectBounce*>(s1->material->i[1]);
			//bounceParam->reflectEnegyRate = 1.0;
		}
	}

	//auto light = new DirectionalLight(P(-1, -1, 0), P(1, 1, 1));
	//auto light = new PointLight(P(2.0, 2.0, -3.0), P(1, 1, 1),1.5);
	//op->PutLight(light);

	op->Evolve();
	op->SaveScreenBufferFrame(screen,"color","D:\\PToyCache\\z.txt");
	//system("D:\\PToyCache\\runTransToImg.bat");
	//op->SayI();
	return 0;
}