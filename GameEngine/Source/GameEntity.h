#pragma once
#include "SceneGraphNode.h"

class GameEntity
{
	SceneGraphNode *_node;
	bool _isDestructible;

public:
	GameEntity(SceneGraphNode*, bool);
	~GameEntity();
	bool isDestructible() const;
	SceneGraphNode* getNode() const;
};

