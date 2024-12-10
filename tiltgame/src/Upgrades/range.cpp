#include "../world.h"
#include "range.h"
Range::Range(float x, float y, int width, int height, EntityType type, string playerName, int price) :Entity(x, y, width, height, type, playerName)
{
	PubSub::subscribe("range", this);
	PubSub::subscribe("action", this);
	this->price = price;
	this->setLevel(1);
	this->setHealth(1);
}

void Range::tick()
{
	if (buyTimer != 0) {
		buyTimer--;
	}
}

void Range::receiveMessage(string channel, string name, void* data)
{

}

bool Range::handleCollision()
{
	for (Entity* entity : collisions) {
		if (entity->getType() == guy && buyTimer == 0) {
			PubSub::publish("entity", "buyUpgrade", this);
		}
	}
	collisions.clear();
	return false;
}