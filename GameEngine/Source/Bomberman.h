#pragma once
#include <vector>
#include "SceneGraphNode.h"
#include "GridMap.h"
#include "Bomb.h"

typedef void(*CallbackType)();

class Bomberman
{
public:
	Bomberman(std::string&& filename, Scene* scene, SceneGraphNode* gameNode, BufferObjects* bufferObjects, Shader* shader, CallbackType activateFlash);
	
	Vector2f getPlayerPosition();

	void playerWalk();
	void rotatePlayerLeft();
	void rotatePlayerRight();
	void rotatePlayerBack();
	void placeBomb();

	void update(unsigned elapsedTime);

private:
	const unsigned WALK_ANIMATION_DURATION = 500;
	const unsigned ROTATE_ANIMATION_DURATION = 400;
	const unsigned BOMB_EXPLOSION_TIME = 3000;
	
	const float BOMB_ANIMATION_SCALE_PARAM = 1.1f;

	const unsigned IDLE_AFTER_TIME = 0;
	const int IDLE_ITERATION_DURATION_MIN = 3000;
	const int IDLE_ITERATION_DURATION_MAX = 5000;
	const unsigned IDLE_ANIMATION_DURATION = 300;

	const unsigned HEAD_RESET_ANIMATION_DURATION = 300;
	const float HEAD_REST_ANGLE_Z = 0;

	const unsigned BOMB_FALL_TIME = 500;
	const float BOMB_FALL_HEIGHT = -1.3f;

	const float WIGGLE_ANIMATION_SPEED = 0.005f;
	const float WIGGLE_ANIMATION_MAX_VALUE = 0.04f;

	const float PLAYER_HEAD_ORIENTATION_MIN = -30.0f;
	const float PLAYER_HEAD_ORIENTATION_MAX = 30.0f;
	
	const float WAVE_ANIMATION_MAX_ROTATION = 20.0f;


	GridMap *grid_map_;
	std::vector<Bomb*> bombs_;
	Vector2f player_position_;
	float player_orientation_;
	GameEntity *player_entity_;

	unsigned total_walk_time_;
	int total_rotation_time_; // TODO
	unsigned total_head_reset_time_;

	int rotation_direction_;
	bool player_active_;
	int starting_foot_;
	CallbackType activate_flash_;
	Bomb* bomb_on_player_;

	unsigned player_inactive_since_;
	unsigned idle_last_iteration_time_;
	int idle_iteration_duration_;
	float head_angle_z_from_;
	float head_angle_z_current_;
	float head_angle_z_to_;

	bool isClearAhead() const;
	void setPlayerActivity(bool activity);

	bool movePlayerForward(float distance);
	void rotatePlayer(float angleDeg);
	void rotateHead(float z_from, float z_to, float anim_percent_cummulative);
	
	void explode(unsigned row, unsigned col);

	void dropBomb();

	void initWalk();

	void liftArms();
	void lowerArms();

	void updateBombs();
	void animationsUpdate(unsigned elapsedTime);
	void bombAnimationsUpdate(unsigned elapsed_time);
	void playerAnimationsUpdate(unsigned elapsed_time);
	void animateBomb(SceneGraphNode* node, float percentage);
	void playerActiveAnimationsUpdate(unsigned elapsed_time);
	void playerResetHeadAnimation(unsigned elapsed_time);
	void playerRotateAndWalkAnimation(unsigned elapsed_time);
	void playerWiggleBodyAnimation(unsigned idle_time);
	void playerIdleAnimationsUpdate();
	void playerLookRandom();
	void wavePlayerMembers(float harmonicPercentage);

	static Vector2f angleTo2D(float angleDeg);
	static unsigned getCurrentTime();
};
