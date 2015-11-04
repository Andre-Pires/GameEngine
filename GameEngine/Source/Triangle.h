#pragma once
#include "GeometricObject.h"

class Triangle : public GeometricObject
{
public:
	Triangle(BufferObjects* buffer, Scene* scene);
	void shadeColor() override;
};
