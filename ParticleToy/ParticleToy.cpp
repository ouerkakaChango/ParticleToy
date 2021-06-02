// ParticleToy.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>

#include "rayTraceRender/rayTraceWorld.h"
#include "rayTraceRender/rayTraceScreen.h"

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

int main()
{
	rayTraceWorld* world = new rayTraceWorld;
	world->SetTraceSettings(1);
	rayTraceWorldR* op = (rayTraceWorldR*)world->r[0];
	op->PutShape(new Sphere(P(0, 0, -5), 1.0),"Sphere1");
	auto screen = new rayTraceScreen(1080,720);
	op->PutScreen(screen);
	op->Evolve();
	//op->SaveScreenDebugFrame(screen,"D:\\PToyCache\\z.txt");
	//op->SayI();
	return 0;
}