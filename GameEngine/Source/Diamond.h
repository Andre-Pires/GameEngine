#pragma once
#include "GeometricObject.h"
#include "Vector3f.h"

class Diamond : public GeometricObject
{
public:
	Diamond(BufferObjects* buffer, Scene* scene);
	void shadeColor() override;
};