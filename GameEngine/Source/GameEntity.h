#pragma once
#include "SceneGraphNode.h"

class GameEntity
{
	SceneGraphNode *node_;
	SceneGraphNode *scaling_node_;
	SceneGraphNode *translation_node_;
	bool is_destructible_;

public:
	GameEntity(SceneGraphNode* node, bool is_destrcutible, SceneGraphNode* scaling_node, SceneGraphNode* translation_node);
	GameEntity(SceneGraphNode* node, bool is_destructible);
	~GameEntity();

	SceneGraphNode* getNode() const;
	SceneGraphNode* getScalingNode() const;
	SceneGraphNode* getTranslationNode() const;

	bool isDestructible() const;
};

