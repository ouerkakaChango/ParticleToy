#pragma once
#include "Taiji.h"

//i[0]:Shape
class Shape;
class Material;
class Object : public Yang
{
	THISY(Object)

	void SetShape(Shape* shape_);

	Shape* shape = nullptr;

	Material* material = nullptr;

	int uid = -1;
	str name="obj";
	static int maxInx;

};