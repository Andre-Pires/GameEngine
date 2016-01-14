#pragma once
#include "GeometricObject.h"

class TriangularPrism : public GeometricObject
{
public:
	TriangularPrism(BufferObjects* buffer, Scene* scene);
	void shadeColor() override;
};
