#ifndef HEALTH_H
#define HEALITH_H

#include "../entity.h"
#include "../pubSub.h"

using namespace std;

class Health: public Entity, public Subscriber {

private:
	int buyTimer = 0;
public:
	Health(float x, float y, int width, int height, EntityType type, string playerName, int price);
	void tick();
	void receiveMessage(string channel, string name, void* data);
	bool handleCollision();
};



#endif