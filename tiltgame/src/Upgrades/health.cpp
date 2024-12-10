#include "../world.h"
#include "health.h"

Health::Health(float x, float y, int width, int height, EntityType type, string playerName, int price) :Entity(x, y, width, height, type, playerName)
{
	PubSub::subscribe("health", this);
	PubSub::subscribe("action", this);
	this->price = price;
	this->setLevel(1);
	this->setHealth(1);
};
void Health::tick() {
	if (buyTimer != 0) {
		buyTimer--;
	}
}


void Health::receiveMessage(string channel, string message, void* data) {

}

bool Health::handleCollision()
{

	for (Entity* entity : collisions) {
		if (entity->getType() == guy && buyTimer == 0) {
			if (entity->getMaxHealth() != entity->getHealth()) {
				PubSub::publish("entity", "buyUpgrade", this);
				buyTimer = 90;
			}
		}
	}
	collisions.clear();
	return false;
}