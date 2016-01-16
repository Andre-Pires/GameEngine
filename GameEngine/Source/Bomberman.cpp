#include "Bomberman.h"
#include "GameManager.h"
#include "Bomb.h"
#include "Animations.h"

Bomberman::Bomberman(std::string&& filename, Scene* scene, SceneGraphNode* gameNode,
	BufferObjects* bufferObjects, Shader* shader, CallbackType activateFlash) :
	_playerPosition(1, 1), _playerOrientation(0), total_walk_time_(WALK_ANIMATION_DURATION),
	total_rotation_time_(ROTATE_ANIMATION_DURATION), _rotationDirection(0), player_active_(false),
	_startingFoot(-1), bomb_on_player_(nullptr), player_inactive_since_(0),
	idle_last_iteration_time_(0), idle_iteration_duration_(0), head_angle_z_from_(0),
	head_angle_z_current_(0), head_angle_z_to_(0), head_angle_x_from_(0), head_angle_x_current_(0),
	head_angle_x_to_(0)
{
	GameManager::getInstance().init(scene, gameNode, bufferObjects, shader);

	_gridMap = new GridMap(filename);
	_playerEntity = _gridMap->getPlayerEntity();
	_playerPosition = Vector2f(_gridMap->getPlayerCol(), _gridMap->getPlayerRow());
	_activateFlash = activateFlash;
}

Bomberman::~Bomberman()
{
}

Vector2f Bomberman::getPlayerPosition()
{
	return _playerPosition;
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
	_rotationDirection = -1;
}

void Bomberman::rotatePlayerRight()
{
	if (player_active_) return;

	setPlayerActivity(true);
	total_rotation_time_ = 0;
	_rotationDirection = 1;
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

bool Bomberman::update(unsigned elapsedTime)
{
	for (auto bomb = _bombs.begin(); bomb < _bombs.end(); )
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
			_gridMap->setEntity((*bomb)->getRow(), (*bomb)->getCol(), nullptr);
			bomb = _bombs.erase(bomb);
		}
		else
		{
			++bomb;
		}
	}

	animationsUpdate(elapsedTime);

	return true;
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

	auto bombRow = _gridMap->getPlayerRow();
	auto bombCol = _gridMap->getPlayerCol();

	liftArms();

	auto bomb_entity = GameManager::getInstance().createBomb(bombCol, -float(bombRow));
	_gridMap->setEntity(bombRow, bombCol, bomb_entity);
	bomb_on_player_ = new Bomb(bomb_entity, getCurrentTime() + BOMB_EXPLOSION_TIME, bombRow, bombCol);
	_bombs.push_back(bomb_on_player_);
}

void Bomberman::setPlayerActivity(bool activity)
{
	if (!player_active_ && activity) // activating
	{
		// Starting head movement (reset), from current to rest
		head_angle_z_from_ = head_angle_z_current_;
		head_angle_x_from_ = head_angle_x_current_;
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
	auto playerDirection = angleTo2D(-_playerOrientation);
	auto deltaPos = playerDirection * distance;
	deltaPos.y = -deltaPos.y;
	auto newPlayerPos = _playerPosition + deltaPos;

	unsigned newRow = round(newPlayerPos.y);
	unsigned newCol = round(newPlayerPos.x);
	unsigned oldRow = _gridMap->getPlayerRow();
	unsigned oldCol = _gridMap->getPlayerCol();

	bool valid = true;

	if (newRow != oldRow || newCol != oldCol)
	{
		valid = _gridMap->isClear(newRow, newCol);

		if (valid)
		{
			_gridMap->setPlayerRow(newRow);
			_gridMap->setPlayerCol(newCol);
		}
	}

	if (valid)
	{
		if (bomb_on_player_ != nullptr) dropBomb();
		_playerEntity->getNode()->translate(Vector3f(deltaPos.x, -deltaPos.y, 0));
		_playerPosition += Vector2f(deltaPos.x, deltaPos.y);
	}

	return valid;
}

void Bomberman::rotatePlayer(float angleDeg)
{
	_playerOrientation += angleDeg;

	_playerEntity->getNode()->clearTransformations();
	_playerEntity->getNode()->rotate(-_playerOrientation, Vector3f(0, 0, 1));
	_playerEntity->getNode()->translate(Vector3f(_playerPosition.x + 0.5f, -_playerPosition.y + 0.5f, 0));
}

void Bomberman::explode(unsigned row, unsigned col)
{
	_activateFlash();

	if (!_gridMap->isClear(row - 1, col))
	{
		if (_gridMap->isDestructible(row - 1, col))
		{
			_gridMap->clear(row - 1, col);
		}
	}
	else
	{
		if (_gridMap->isDestructible(row - 2, col))
		{
			_gridMap->clear(row - 2, col);
		}
	}

	if (!_gridMap->isClear(row + 1, col))
	{
		if (_gridMap->isDestructible(row + 1, col))
		{
			_gridMap->clear(row + 1, col);
		}
	}
	else
	{
		if (_gridMap->isDestructible(row + 2, col))
		{
			_gridMap->clear(row + 2, col);
		}
	}

	if (!_gridMap->isClear(row, col - 1))
	{
		if (_gridMap->isDestructible(row, col - 1))
		{
			_gridMap->clear(row, col - 1);
		}
	}
	else
	{
		if (_gridMap->isDestructible(row, col - 2))
		{
			_gridMap->clear(row, col - 2);
		}
	}

	if (!_gridMap->isClear(row, col + 1))
	{
		if (_gridMap->isDestructible(row, col + 1))
		{
			_gridMap->clear(row, col + 1);
		}
	}
	else
	{
		if (_gridMap->isDestructible(row, col + 2))
		{
			_gridMap->clear(row, col + 2);
		}
	}
}

void Bomberman::animationsUpdate(unsigned elapsed_time)
{
	playerUpdate(elapsed_time);

	for (auto &bomb : _bombs)
	{
		auto timeUntillExplosion = bomb->getExplosionTime() - getCurrentTime();
		auto timeSincePlaced = BOMB_EXPLOSION_TIME - timeUntillExplosion;
		animateBomb(bomb->getEntity()->getScalingNode(), float(timeSincePlaced) / BOMB_EXPLOSION_TIME);

		if (bomb->getDroppedTime() > 0)
		{
			auto time_since_dropped = getCurrentTime() - bomb->getDroppedTime();
			auto percentage_animation = time_since_dropped / float(BOMB_FALL_TIME);
			if (percentage_animation < 1)
				Animations::freeFalling(bomb->getEntity()->getTranslationNode(), -1.3f, percentage_animation);
			else
				bomb->setDroppedTime(0);
		}
	}
}

void Bomberman::rotateHead(float z_from, float z_to, float x_from, float x_to, float anim_percent_cummulative)
{
	head_angle_z_current_ = Animations::lerp(z_from, z_to, anim_percent_cummulative);
	//head_angle_x_current_ = Animations::lerp(x_from, x_to, anim_percent_cummulative);

	GameManager::getInstance().getHead()->clearTransformations();
	GameManager::getInstance().getHead()->rotate(head_angle_z_current_, Vector3f(0, 0, 1));
	//GameManager::getInstance().getHead()->rotate(head_angle_x_current_, Vector3f(1, 0, 0));
}

void Bomberman::playerUpdate(unsigned elapsed_time)
{
	if (player_active_)
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

				rotateHead(head_angle_z_from_, HEAD_REST_ANGLE_Z, head_angle_x_from_, HEAD_REST_ANGLE_X, anim_percent_cummulative);
			}
		}

		auto rotation_finished = (total_rotation_time_ == ROTATE_ANIMATION_DURATION);

		if (!rotation_finished)
		{
			unsigned rotationTime;
			if (total_rotation_time_ + elapsed_time >= ROTATE_ANIMATION_DURATION)
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
				auto frameAngle = percentageOfAnimation * 90 * _rotationDirection;
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
			elapsed_time -= walkTime;

			if (walkTime > 0)
			{
				total_walk_time_ += walkTime;
				float percentageOfAnimation = float(walkTime) / WALK_ANIMATION_DURATION;
				movePlayerForward(percentageOfAnimation);
				wavePlayerMembers(sin(float(total_walk_time_) / WALK_ANIMATION_DURATION * PI) * _startingFoot);
			}
		}

		setPlayerActivity(!rotation_finished || !walking_finished);
	}
	else
	{
		auto duration_of_inactivity = getCurrentTime() - player_inactive_since_;
		if (duration_of_inactivity > IDLE_AFTER_TIME)
		{
			auto idle_time = duration_of_inactivity - IDLE_AFTER_TIME;

			// TODO
			auto ammount = sin(idle_time*0.005f)*0.04f;
			GameManager::getInstance().getBody()->clearTransformations();
			GameManager::getInstance().getBody()->translate(Vector3f(0, 0, ammount));


			//auto total_animation_time = duration_of_inactivity - IDLE_AFTER_TIME;
			auto time_since_last_iteration = getCurrentTime() - idle_last_iteration_time_;

			if (time_since_last_iteration > idle_iteration_duration_)
			{
				head_angle_z_from_ = head_angle_z_current_;
				head_angle_x_from_ = head_angle_x_current_;

				//head_angle_z_to_ = std::max(-30.0f, std::min(30.0f, Utilities::random_normal(head_angle_z_current_, 5)));
				head_angle_z_to_ = Utilities::random_uniform(-30.0f, 30.0f);
				//head_angle_x_to_ = Utilities::random_normal(-15.0f, 15.0f);

				idle_last_iteration_time_ = getCurrentTime();
				time_since_last_iteration = 0;
				idle_iteration_duration_ = Utilities::random_uniform(3000, 5000);
			}

			auto anim_percent_cummulative = std::min(1.0f, time_since_last_iteration / float(IDLE_ANIMATION_DURATION));

			rotateHead(head_angle_z_from_, head_angle_z_to_, head_angle_x_from_, head_angle_x_to_, anim_percent_cummulative);
		}
	}
}

void Bomberman::wavePlayerMembers(float harmonicPercentage)
{
	auto rotAxis = Vector3f(1, 0, 0);
	auto rotMultiplier = 20;

	GameManager::getInstance().getRightHand()->clearTransformations();
	GameManager::getInstance().getRightHand()->rotate(harmonicPercentage * rotMultiplier, rotAxis);

	GameManager::getInstance().getLeftHand()->clearTransformations();
	GameManager::getInstance().getLeftHand()->rotate(-harmonicPercentage * rotMultiplier, rotAxis);

	GameManager::getInstance().getRightFoot()->clearTransformations();
	GameManager::getInstance().getRightFoot()->rotate(-harmonicPercentage * rotMultiplier, rotAxis);

	GameManager::getInstance().getLeftFoot()->clearTransformations();
	GameManager::getInstance().getLeftFoot()->rotate(harmonicPercentage * rotMultiplier, rotAxis);
}

void Bomberman::animateBomb(SceneGraphNode* node, float percentage)
{
	auto scale = 1 - sin(percentage * 1.1f * PI) * percentage*percentage;

	node->clearTransformations();
	node->scale(Vector3f(scale, scale, scale));
}

void Bomberman::dropBomb()
{
	bomb_on_player_->setDroppedTime(getCurrentTime());
	bomb_on_player_ = nullptr;
}

void Bomberman::initWalk()
{
	setPlayerActivity(true);
	_startingFoot = -_startingFoot;
	total_walk_time_ = 0;
}

bool Bomberman::isClearAhead() const
{
	auto playerOrientation2D = angleTo2D(-_playerOrientation);
	unsigned rowAhead = _gridMap->getPlayerRow() - round(playerOrientation2D.y);
	unsigned colAhead = _gridMap->getPlayerCol() + round(playerOrientation2D.x);

	return _gridMap->isClear(rowAhead, colAhead);
}
