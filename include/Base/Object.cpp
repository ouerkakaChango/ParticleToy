#include "Object.h"

#include "FastGeo.h"

int Object::maxInx = 0;

Object::Object()
{
	uid = maxInx;
	name += uid;
	maxInx++;
}

void Object::SetShape(Shape* shape_)
{
	shape = shape_;
}