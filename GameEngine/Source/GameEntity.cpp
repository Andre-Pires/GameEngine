#include "GameEntity.h"


GameEntity::GameEntity(SceneGraphNode* node, bool is_destrcutible, SceneGraphNode* scaling_node, SceneGraphNode* translation_node) : node_(node), scaling_node_(scaling_node), translation_node_(translation_node), is_destructible_(is_destrcutible)
{
}

GameEntity::GameEntity(SceneGraphNode* node, bool is_destrcutible) : GameEntity(node, is_destrcutible, nullptr, nullptr)
{
}


GameEntity::~GameEntity()
{
	delete node_;
}

bool GameEntity::isDestructible() const
{
	return is_destructible_;
}

SceneGraphNode* GameEntity::getNode() const
{
	return node_;
}

SceneGraphNode* GameEntity::getScalingNode() const
{
	return scaling_node_;
}

SceneGraphNode* GameEntity::getTranslationNode() const
{
	return translation_node_;
}
