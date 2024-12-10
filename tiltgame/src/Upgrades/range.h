#ifndef RANGE_H
#define RANGE_H

#include "../entity.h"
#include "../pubSub.h"

using namespace std;

class Range : public Entity, public Subscriber {
private:
	int buyTimer = 0;
public:
	Range(float x, float y, int width, int height, EntityType type, string playerName, int price);
	void tick();
	void receiveMessage(string channel, string name, void* data);
	bool handleCollision();
};



#endif