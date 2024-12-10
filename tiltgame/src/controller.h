#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "pubsub.h"
#include "world.h"
#include "gameView.h"
#include "worldView.h"
#include "textures.h";
#include "settings.h"

class Controller : public Subscriber {

private:
	World world;
	vector<EntityView* > views;

public:
	void gameLoop();
	void receiveMessage(string channel, string message, void* data);
};


#endif