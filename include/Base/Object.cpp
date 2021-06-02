#include "Object.h"

#include "FastGeo.h"
#include "Render/Material.h"

int Object::maxInx = 0;

Object::Object()
{
	uid = maxInx;
	name += uid;
	maxInx++;
	material = new Material;
}

void Object::SetShape(Shape* shape_)
{
	shape = shape_;
}