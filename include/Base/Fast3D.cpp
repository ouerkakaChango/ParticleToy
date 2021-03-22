#include "Fast3D.h"

P::P()
{

}

P::P(double x_, double y_, double z_):x(x_),y(y_),z(z_)
{

}

Pnt::Pnt()
{

}

Pnt::Pnt(P pos_):pos(pos_)
{

}

void Fast3D::AddPnt(str name, P pos)
{
	pnts += pos;
}