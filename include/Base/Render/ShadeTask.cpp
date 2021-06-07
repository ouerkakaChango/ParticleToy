#include "ShadeTask.h"

ShadeTask::ShadeTask(Material* material_, const P& p_, const P& n_, const P& v_, const arr<LightInfo>& lightInfos_)
	: material(material_), p(p_), n(n_), v(v_), lightInfos(lightInfos_)
{

}

P ShadeTask::Calculate()
{
	return material->Calculate(lightInfos,n,v,control);
}