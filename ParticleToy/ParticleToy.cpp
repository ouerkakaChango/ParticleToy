// ParticleToy.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>

#include "../include/Base/Spaces.h"

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

int main()
{


	return 0;
}