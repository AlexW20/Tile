#ifndef MOVEMENT_H
#define MOVEMENT_H

#include "../entity.h"
#include "../pubSub.h"

using namespace std;

class movement : public Entity, public Subscriber {

private:
	int buyTimer = 0;
public:
	movement(float x, float y, int width, int height, EntityType type, string playerName, int price);
	void tick();
	void receiveMessage(string channel, string name, void* data);
	bool handleCollision();
};



#endif