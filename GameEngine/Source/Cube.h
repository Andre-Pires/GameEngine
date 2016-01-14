#pragma once
#include "GeometricObject.h"

class Cube : public GeometricObject
{
public:
	Cube(BufferObjects* buffer, Scene* scene);
	void shadeColor() override;
};