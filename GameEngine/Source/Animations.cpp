#include "Animations.h"

void Animations::freeFalling(SceneGraphNode* node, float final_height, float percent)
{
	auto height_diff = final_height + (1 - percent) * (-final_height) * abs(cos(percent * 2 * PI));
	
	node->clearTransformations();
	node->translate(Vector3f(0, 0, height_diff));
}

float Animations::lerp(float from, float to, float percent)
{
	return (1 - percent) * from + percent * to;
}
