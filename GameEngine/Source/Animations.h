#pragma once

#include "SceneGraphNode.h"

namespace Animations
{
	void freeFalling(SceneGraphNode* node, float final_height, float percent);

	float lerp(float from, float to, float percent);
}