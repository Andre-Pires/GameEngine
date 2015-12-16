#include "GameEntity.h"



GameEntity::GameEntity(SceneGraphNode* node, bool isDestrcutible) : _node(node), _isDestructible(isDestrcutible)
{
}


GameEntity::~GameEntity()
{
	delete _node;
}

bool GameEntity::isDestructible() const
{
	return _isDestructible;
}

SceneGraphNode* GameEntity::getNode() const
{
	return _node;
}
