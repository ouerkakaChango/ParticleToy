#pragma once

enum MaterialMode
{
	MaterialMode_BlinnPhong,
};
class Material
{
public:
	MaterialMode mode = MaterialMode_BlinnPhong;
};