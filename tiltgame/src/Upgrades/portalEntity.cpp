#include "portalEntity.h"
#include "../world.h"

PortalEntity::PortalEntity(float x, float y, int width, int height, EntityType type, string playerName, int price) :Entity(x, y, width, height, type, playerName)
{
	PubSub::subscribe("portal", this);
	PubSub::subscribe("action", this);
	this->price = price;
	this->setLevel(1);
	this->setHealth(1);

};
void PortalEntity::tick() {
	if (buyTimer != 0) {
		buyTimer--;
	}
}


void PortalEntity::receiveMessage(string channel, string message, void* data) {

}

bool PortalEntity::handleCollision()
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