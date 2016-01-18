#include "Bomberman.h"
#include "GameManager.h"
#include "Bomb.h"
#include "Animations.h"

Bomberman::Bomberman(std::string&& filename, Scene* scene, SceneGraphNode* gameNode,
	BufferObjects* bufferObjects, Shader* shader, CallbackType activateFlash) :
	player_position_(1, 1), player_orientation_(0), total_walk_time_(WALK_ANIMATION_DURATION),
	total_rotation_time_(ROTATE_ANIMATION_DURATION), total_head_reset_time_(0), 
	rotation_direction_(0), player_active_(false), starting_foot_(-1), bomb_on_player_(nullptr), 
	player_inactive_since_(0), idle_last_iteration_time_(0), idle_iteration_duration_(0), 
	head_angle_z_from_(0), head_angle_z_current_(0), head_angle_z_to_(0)
{
	GameManager::getInstance().init(scene, gameNode, bufferObjects, shader);

	grid_map_ = new GridMap(filename);
	player_entity_ = grid_map_->getPlayerEntity();
	player_position_ = Vector2f(grid_map_->getPlayerCol(), grid_map_->getPlayerRow());
	activate_flash_ = activateFlash;
}

Vector2f Bomberman::getPlayerPosition()
{
	return player_position_;
}

void Bomberman::playerWalk()
{
	if (player_active_) return;

	if (isClearAhead())
	{
		initWalk();
	}
}

void Bomberman::rotatePlayerLeft()
{
	if (player_active_) return;

	setPlayerActivity(true);
	total_rotation_time_ = 0;
	rotation_direction_ = -1;
}

void Bomberman::rotatePlayerRight()
{
	if (player_active_) return;

	setPlayerActivity(true);
	total_rotation_time_ = 0;
	rotation_direction_ = 1;
}

void Bomberman::rotatePlayerBack()
{
	if (player_active_) return;

	setPlayerActivity(true);
	total_rotation_time_ = -int(ROTATE_ANIMATION_DURATION);
	rotation_direction_ = 1;
}

void Bomberman::liftArms()
{
	GameManager::getInstance().getRightHand()->clearTransformations();
	GameManager::getInstance().getRightHand()->translate(Vector3f(0, 0, 0.5f));

	GameManager::getInstance().getLeftHand()->clearTransformations();
	GameManager::getInstance().getLeftHand()->translate(Vector3f(0, 0, 0.5f));
}

void Bomberman::lowerArms()
{
	GameManager::getInstance().getRightHand()->clearTransformations();
	GameManager::getInstance().getLeftHand()->clearTransformations();
}

void Bomberman::updateBombs()
{
	for (auto bomb = bombs_.begin(); bomb < bombs_.end(); )
	{
		if ((*bomb)->getExplosionTime() <= getCurrentTime())
		{
			explode((*bomb)->getRow(), (*bomb)->getCol());

			if (*bomb == bomb_on_player_)
			{
				bomb_on_player_ = nullptr;
				lowerArms();
			}

			delete (*bomb)->getEntity();
			grid_map_->setEntity((*bomb)->getRow(), (*bomb)->getCol(), nullptr);
			bomb = bombs_.erase(bomb);
		}
		else
		{
			++bomb;
		}
	}
}

void Bomberman::update(unsigned elapsedTime)
{
	updateBombs();
	animationsUpdate(elapsedTime);
}

Vector2f Bomberman::angleTo2D(float angleDeg)
{
	float rad = (angleDeg * PI) / 180;

	return Vector2f(cos(rad), sin(rad));
}

unsigned Bomberman::getCurrentTime()
{
	return glutGet(GLUT_ELAPSED_TIME);
}

void Bomberman::placeBomb()
{
	if (player_active_ || bomb_on_player_ != nullptr) return;
	setPlayerActivity(true);

	auto bombRow = grid_map_->getPlayerRow();
	auto bombCol = grid_map_->getPlayerCol();

	liftArms();

	auto bomb_entity = GameManager::getInstance().createBomb(bombCol, -float(bombRow));
	grid_map_->setEntity(bombRow, bombCol, bomb_entity);
	bomb_on_player_ = new Bomb(bomb_entity, getCurrentTime() + BOMB_EXPLOSION_TIME, bombRow, bombCol);
	bombs_.push_back(bomb_on_player_);
}

void Bomberman::setPlayerActivity(bool activity)
{
	if (!player_active_ && activity) // activating
	{
		// Starting head movement (reset), from current to rest
		head_angle_z_from_ = head_angle_z_current_;
	}
	else if (player_active_ && !activity) // disactivating
	{
		player_inactive_since_ = getCurrentTime();
		idle_last_iteration_time_ = 0;
		total_head_reset_time_ = 0;
		GameManager::getInstance().getBody()->clearTransformations();
	}

	player_active_ = activity;
}

bool Bomberman::movePlayerForward(float distance)
{
	auto playerDirection = angleTo2D(-player_orientation_);
	auto deltaPos = playerDirection * distance;
	deltaPos.y = -deltaPos.y;
	auto newPlayerPos = player_position_ + deltaPos;

	unsigned newRow = round(newPlayerPos.y);
	unsigned newCol = round(newPlayerPos.x);
	unsigned oldRow = grid_map_->getPlayerRow();
	unsigned oldCol = grid_map_->getPlayerCol();

	bool valid = true;

	if (newRow != oldRow || newCol != oldCol)
	{
		valid = grid_map_->isClear(newRow, newCol);

		if (valid)
		{
			grid_map_->setPlayerRow(newRow);
			grid_map_->setPlayerCol(newCol);
		}
	}

	if (valid)
	{
		if (bomb_on_player_ != nullptr) dropBomb();
		player_entity_->getNode()->translate(Vector3f(deltaPos.x, -deltaPos.y, 0));
		player_position_ += Vector2f(deltaPos.x, deltaPos.y);
	}

	return valid;
}

void Bomberman::rotatePlayer(float angleDeg)
{
	player_orientation_ += angleDeg;

	player_entity_->getNode()->clearTransformations();
	player_entity_->getNode()->rotate(-player_orientation_, Vector3f(0, 0, 1));
	player_entity_->getNode()->translate(Vector3f(player_position_.x + 0.5f, -player_position_.y + 0.5f, 0));
}

void Bomberman::explode(unsigned row, unsigned col)
{
	activate_flash_();

	if (!grid_map_->isClear(row - 1, col))
	{
		if (grid_map_->isDestructible(row - 1, col))
		{
			grid_map_->clear(row - 1, col);
		}
	}
	else
	{
		if (grid_map_->isDestructible(row - 2, col))
		{
			grid_map_->clear(row - 2, col);
		}
	}

	if (!grid_map_->isClear(row + 1, col))
	{
		if (grid_map_->isDestructible(row + 1, col))
		{
			grid_map_->clear(row + 1, col);
		}
	}
	else
	{
		if (grid_map_->isDestructible(row + 2, col))
		{
			grid_map_->clear(row + 2, col);
		}
	}

	if (!grid_map_->isClear(row, col - 1))
	{
		if (grid_map_->isDestructible(row, col - 1))
		{
			grid_map_->clear(row, col - 1);
		}
	}
	else
	{
		if (grid_map_->isDestructible(row, col - 2))
		{
			grid_map_->clear(row, col - 2);
		}
	}

	if (!grid_map_->isClear(row, col + 1))
	{
		if (grid_map_->isDestructible(row, col + 1))
		{
			grid_map_->clear(row, col + 1);
		}
	}
	else
	{
		if (grid_map_->isDestructible(row, col + 2))
		{
			grid_map_->clear(row, col + 2);
		}
	}
}

void Bomberman::bombAnimationsUpdate(unsigned elapsed_time)
{
	for (auto &bomb : bombs_)
	{
		auto timeUntillExplosion = bomb->getExplosionTime() - getCurrentTime();
		auto timeSincePlaced = BOMB_EXPLOSION_TIME - timeUntillExplosion;
		animateBomb(bomb->getEntity()->getScalingNode(), float(timeSincePlaced) / BOMB_EXPLOSION_TIME);

		if (bomb->getDroppedTime() > 0)
		{
			auto time_since_dropped = getCurrentTime() - bomb->getDroppedTime();
			auto percentage_animation = time_since_dropped / float(BOMB_FALL_TIME);
			if (percentage_animation < 1)
				Animations::freeFalling(bomb->getEntity()->getTranslationNode(), BOMB_FALL_HEIGHT, percentage_animation);
			else
				bomb->setDroppedTime(0);
		}
	}
}

void Bomberman::animationsUpdate(unsigned elapsed_time)
{
	playerAnimationsUpdate(elapsed_time);
	bombAnimationsUpdate(elapsed_time);
}

void Bomberman::rotateHead(float z_from, float z_to, float anim_percent_cummulative)
{
	head_angle_z_current_ = Animations::lerp(z_from, z_to, anim_percent_cummulative);

	GameManager::getInstance().getHead()->clearTransformations();
	GameManager::getInstance().getHead()->rotate(head_angle_z_current_, Vector3f(0, 0, 1));
}

void Bomberman::playerResetHeadAnimation(unsigned elapsed_time)
{
	auto head_reset_finished = (total_head_reset_time_ == HEAD_RESET_ANIMATION_DURATION);

	if (!head_reset_finished)
	{
		unsigned anim_time;
		if (total_head_reset_time_ + elapsed_time >= HEAD_RESET_ANIMATION_DURATION)
		{
			// This means the rotation is finishing, we should walk next
			anim_time = HEAD_RESET_ANIMATION_DURATION - total_head_reset_time_;
			head_reset_finished = true;
		}
		else
		{
			anim_time = elapsed_time;
		}

		if (anim_time > 0)
		{
			total_head_reset_time_ += anim_time;
			auto anim_percent_cummulative = total_head_reset_time_ / float(HEAD_RESET_ANIMATION_DURATION);

			rotateHead(head_angle_z_from_, HEAD_REST_ANGLE_Z, anim_percent_cummulative);
		}
	}
}

void Bomberman::playerRotateAndWalkAnimation(unsigned elapsed_time)
{
	auto rotation_finished = (total_rotation_time_ == ROTATE_ANIMATION_DURATION);

	if (!rotation_finished)
	{
		unsigned rotationTime;
		int test = total_rotation_time_ + int(elapsed_time);
		if (test >= int(ROTATE_ANIMATION_DURATION))
		{
			// This means the rotation is finishing, we should walk next
			rotationTime = ROTATE_ANIMATION_DURATION - total_rotation_time_;
			rotation_finished = true;
		}
		else
		{
			rotationTime = elapsed_time;
		}
		elapsed_time -= rotationTime;

		if (rotationTime > 0)
		{
			total_rotation_time_ += rotationTime;
			float percentageOfAnimation = float(rotationTime) / ROTATE_ANIMATION_DURATION;
			auto frameAngle = percentageOfAnimation * 90 * rotation_direction_;
			rotatePlayer(frameAngle);
		}

		// If rotation has just finished and the cell ahead is clear, we're walking next
		if (rotation_finished && isClearAhead())
		{
			initWalk();
		}
	}

	auto walking_finished = (total_walk_time_ == WALK_ANIMATION_DURATION);

	if (rotation_finished && !walking_finished && elapsed_time > 0)
	{
		unsigned walkTime;
		if (total_walk_time_ + elapsed_time >= WALK_ANIMATION_DURATION)
		{
			walkTime = WALK_ANIMATION_DURATION - total_walk_time_;
			walking_finished = true;
		}
		else
		{
			walkTime = elapsed_time;
		}

		if (walkTime > 0)
		{
			total_walk_time_ += walkTime;
			float percentageOfAnimation = float(walkTime) / WALK_ANIMATION_DURATION;
			movePlayerForward(percentageOfAnimation);

			auto harmonic_percentage = sin(float(total_walk_time_) / WALK_ANIMATION_DURATION * PI) * starting_foot_;
			wavePlayerMembers(harmonic_percentage);
		}
	}

	setPlayerActivity(!rotation_finished || !walking_finished);
}

void Bomberman::playerActiveAnimationsUpdate(unsigned elapsed_time)
{
	playerResetHeadAnimation(elapsed_time);
	playerRotateAndWalkAnimation(elapsed_time);
}

void Bomberman::playerWiggleBodyAnimation(unsigned idle_time)
{
	auto ammount = sin(idle_time * WIGGLE_ANIMATION_SPEED) * WIGGLE_ANIMATION_MAX_VALUE;
	GameManager::getInstance().getBody()->clearTransformations();
	GameManager::getInstance().getBody()->translate(Vector3f(0, 0, ammount));
}

void Bomberman::playerLookRandom()
{
	auto time_since_last_iteration = getCurrentTime() - idle_last_iteration_time_;

	if (time_since_last_iteration > idle_iteration_duration_)
	{
		head_angle_z_from_ = head_angle_z_current_;

		head_angle_z_to_ = Utilities::random_uniform(PLAYER_HEAD_ORIENTATION_MIN, PLAYER_HEAD_ORIENTATION_MAX);

		idle_last_iteration_time_ = getCurrentTime();
		time_since_last_iteration = 0;
		idle_iteration_duration_ = Utilities::random_uniform(IDLE_ITERATION_DURATION_MIN, IDLE_ITERATION_DURATION_MAX);
	}

	auto anim_percent_cummulative = std::min(1.0f, time_since_last_iteration / float(IDLE_ANIMATION_DURATION));

	rotateHead(head_angle_z_from_, head_angle_z_to_, anim_percent_cummulative);
}

void Bomberman::playerIdleAnimationsUpdate()
{
	auto duration_of_inactivity = getCurrentTime() - player_inactive_since_;
	if (duration_of_inactivity > IDLE_AFTER_TIME)
	{
		auto idle_time = duration_of_inactivity - IDLE_AFTER_TIME;

		playerWiggleBodyAnimation(idle_time);
		playerLookRandom();
	}
}

void Bomberman::playerAnimationsUpdate(unsigned elapsed_time)
{
	if (player_active_)
	{
		playerActiveAnimationsUpdate(elapsed_time);
	}
	else
	{
		playerIdleAnimationsUpdate();
	}
}

void Bomberman::wavePlayerMembers(float harmonicPercentage)
{
	auto rotAxis = Vector3f(1, 0, 0);

	GameManager::getInstance().getRightHand()->clearTransformations();
	GameManager::getInstance().getRightHand()->rotate(harmonicPercentage * WAVE_ANIMATION_MAX_ROTATION, rotAxis);

	GameManager::getInstance().getLeftHand()->clearTransformations();
	GameManager::getInstance().getLeftHand()->rotate(-harmonicPercentage * WAVE_ANIMATION_MAX_ROTATION, rotAxis);

	GameManager::getInstance().getRightFoot()->clearTransformations();
	GameManager::getInstance().getRightFoot()->rotate(-harmonicPercentage * WAVE_ANIMATION_MAX_ROTATION, rotAxis);

	GameManager::getInstance().getLeftFoot()->clearTransformations();
	GameManager::getInstance().getLeftFoot()->rotate(harmonicPercentage * WAVE_ANIMATION_MAX_ROTATION, rotAxis);
}

void Bomberman::animateBomb(SceneGraphNode* node, float percentage)
{
	auto scale = 1 - sin(percentage * BOMB_ANIMATION_SCALE_PARAM * PI) * percentage*percentage;

	node->clearTransformations();
	node->scale(Vector3f(scale));
}

void Bomberman::dropBomb()
{
	bomb_on_player_->setDroppedTime(getCurrentTime());
	bomb_on_player_ = nullptr;
}

void Bomberman::initWalk()
{
	setPlayerActivity(true);
	starting_foot_ = -starting_foot_;
	total_walk_time_ = 0;
}

bool Bomberman::isClearAhead() const
{
	auto playerOrientation2D = angleTo2D(-player_orientation_);
	unsigned rowAhead = grid_map_->getPlayerRow() - round(playerOrientation2D.y);
	unsigned colAhead = grid_map_->getPlayerCol() + round(playerOrientation2D.x);

	return grid_map_->isClear(rowAhead, colAhead);
}
