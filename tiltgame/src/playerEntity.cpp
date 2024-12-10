#include "playerEntity.h"
#include "world.h"
#include "raylib.h"
using namespace std;

PlayerEntity::PlayerEntity(float x, float y, int width, int height, EntityType type, string playerName, float health) :Entity(x, y, width, height, type, playerName) {
	setHealth(health);
	setMaxHealth(health);
	this->setSpeedX(2);
	this->setSpeedY(2);
	this->setPower(2);
	PubSub::subscribe("action", this);
	PubSub::subscribe("stations", this);
}
PlayerEntity::~PlayerEntity() {
	PubSub::unsubscribe("action", this);
	PubSub::unsubscribe("stations", this);
}

void PlayerEntity::tick()
{

	if (getMoving()) {

		if (movementCountdown > 0)
		{

			movementCountdown--;

			if (movementCountdown == 0)
			{
				setMoving(false);


			}
			else
			{
				switch (getFacing()) {
				case right:
					setXPos(getXPos() + this->getSpeedX());
					break;
				case left:
					setXPos(getXPos() - this->getSpeedX());

					break;
				case up:
					setYPos(getYPos() - this->getSpeedY());
					break;
				case down:
					setYPos(getYPos() + this->getSpeedY());
					break;


				}

			}
		}
	}
}
void PlayerEntity::receiveMessage(string channel, string message, void* data) {

	if (channel == "Upgrades" && message == "bought") {
		Entity* station = (Entity*)data;
		switch (station->getType()) {
		case portal:
			this->setXPos(GetRandomValue(0 + 20 + getWidth(), screenWidth - 20 - getWidth()));
			this->setYPos(GetRandomValue(0 + 20 + getHeight(), screenHeight - 20 - getHeight()));
			break;
		case range:
			this->setAttackRange(this->getAttackRange() + (5 * station->getLevel()));
			break;
		case health:
			this->setMaxHealth(this->getMaxHealth() * 1.5);
			this->setHealth(this->getMaxHealth());
			break;
		case damage:
			this->setPower(this->getPower() * 1.5);
			break;
		case movement:
			this->setSpeedX(this->getSpeedX() + 1);
			this->setSpeedY(this->getSpeedY() + 1);

		}
	}
	if (channel == "action" && message == "player") {
		userAction* action = (userAction*)data;
		if (*action == PlayerHit) {
			PubSub::publish("action", "attack", this);
		}
		if (*action == PlayerRight)
		{
			if (getXPos() < screenWidth - getWidth()) {
				setMoving(true);
				setFacing(right);
				movementCountdown = 2;
			}
		}

		if (*action == PlayerLeft)
		{
			if (getXPos() > 0) {

				setMoving(true);
				setFacing(left);
				movementCountdown = 2;
			}
		}



		if (*action == PlayerUp)
		{
			if (getYPos() > 0) {

				setMoving(true);
				setFacing(up);
				movementCountdown = 2;
			}
		}

		if (*action == PlayerDown)
		{
			if (getYPos() < screenHeight - getHeight()) {

				setMoving(true);
				setFacing(down);
				movementCountdown = 2;
			}
		}

	}
}
bool PlayerEntity::handleCollision() {
	for (Entity* entity : collisions) {

		if (entity->getType() == enemy) {
			setMoving(false);
			movementCountdown = 0;
		}
	}
	collisions.clear();
	return false;
}

float PlayerEntity::getAttackRange()
{
	return this->attackRange;
}

void PlayerEntity::setAttackRange(float attackRange)
{
	this->attackRange = attackRange;
}