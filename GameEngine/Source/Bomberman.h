#pragma once
#include <vector>
#include "SceneGraphNode.h"
#include <string>
#include <fstream>
#include <cassert>
#include "Cube.h"
#include "Diamond.h"
#include "TriangularPrism.h"
#include "GridMap.h"
#include "Bomb.h"

typedef void(*CallbackType)();

class Bomberman
{
private:
	const float _moveStep = 1;
	const unsigned WALK_ANIMATION_DURATION = 500;
	const unsigned ROTATE_ANIMATION_DURATION = 400;
	const unsigned BOMB_EXPLOSION_TIME = 3000;
	const unsigned BOMB_FALL_TIME = 500;
	const unsigned IDLE_AFTER_TIME = 2000;
	const unsigned IDLE_ANIMATION_DURATION = 300;
	const unsigned IDLE_ITERATION_DURATION_MEAN = 4000;
	const unsigned IDLE_ITERATION_DURATION_STDDEV = 1000;
	const unsigned HEAD_RESET_ANIMATION_DURATION = 300;

	const float HEAD_REST_ANGLE_Z = 0;
	const float HEAD_REST_ANGLE_X = 0;

	GridMap *_gridMap;
	std::vector<Bomb*> _bombs;
	Vector2f _playerPosition;
	float _playerOrientation;
	GameEntity *_playerEntity;

	unsigned total_walk_time_;
	unsigned total_rotation_time_;
	unsigned total_head_reset_time_;

	int _rotationDirection;
	bool player_active_;
	int _startingFoot;
	CallbackType _activateFlash;
	Bomb* bomb_on_player_;

	unsigned player_inactive_since_;
	unsigned idle_last_iteration_time_;
	int idle_iteration_duration_;
	float head_angle_z_from_;
	float head_angle_z_current_;
	float head_angle_z_to_;
	float head_angle_x_from_;
	float head_angle_x_current_;
	float head_angle_x_to_;

	void setPlayerActivity(bool activity);

	bool movePlayerForward(float distance);
	void rotatePlayer(float angleDeg);
	void rotateHead(float z_from, float z_to, float x_from, float x_to, float anim_percent_cummulative);

	void explode(unsigned row, unsigned col);
	void animationsUpdate(unsigned elapsedTime);

	void playerUpdate(unsigned elapsed_time);
	void wavePlayerMembers(float harmonicPercentage);
	void animateBomb(SceneGraphNode* node, float percentage);

	void dropBomb();

	void initWalk();
	bool isClearAhead() const;
public:
	Bomberman(std::string&& filename, Scene* scene, SceneGraphNode* gameNode, BufferObjects* bufferObjects, Shader* shader, CallbackType activateFlash);
	~Bomberman();
	Vector2f getPlayerPosition();
	void playerWalk();
	void playerWalkBackwards() { movePlayerForward(-_moveStep); }

	void rotatePlayerLeft();
	void rotatePlayerRight();

	void liftArms();
	void lowerArms();

	void placeBomb();
	bool update(unsigned elapsedTime);

	static Vector2f angleTo2D(float angleDeg);
	static unsigned getCurrentTime();
};
