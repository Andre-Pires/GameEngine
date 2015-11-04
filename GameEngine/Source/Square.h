#pragma once
#include "GeometricObject.h"

class Square : public GeometricObject
{
public:
	Square(BufferObjects* buffer, Scene* scene);
	void shadeColor() override;
};