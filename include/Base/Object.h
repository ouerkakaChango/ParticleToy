#pragma once
#include "Taiji.h"

//i[0]:Shape
class Shape;
class Object : public Yang
{
	THISY(Object)

	void SetShape(Shape* shape_);

	Shape* shape;

	int uid = -1;
	str name="obj";
	static int maxInx;
};