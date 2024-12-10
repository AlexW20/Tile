#include "../world.h"
#include "damage.h"

Damage::Damage(float x, float y, int width, int height, EntityType type, string playerName, int price) :Entity(x, y, width, height, type, playerName)
{
	PubSub::subscribe("damage", this);
	PubSub::subscribe("action", this);
	this->price = price;
	this->setLevel(1);
	this->setHealth(1);
};
void Damage::tick() {
	if (buyTimer != 0) {
		buyTimer--;
	}
}


void Damage::receiveMessage(string channel, string message, void* data) {

}

bool Damage::handleCollision()
{
	for (Entity* entity : collisions) {
		if (entity->getType() == guy && buyTimer == 0) {
			PubSub::publish("entity", "buyUpgrade", this);
			buyTimer = 90;
		}
	}
	collisions.clear();
	return false;
}