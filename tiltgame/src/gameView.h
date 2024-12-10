#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include "raylib.h"
#include "entity.h"
#include "world.h";
#include "playerEntity.h"

class EntityView
{
private:
	Entity* entity;
	Texture2D texture;
	int frame;
	int delay;

public:
	EntityView(Entity* entity);
	bool isViewFor(Entity* entity);
	void draw();
};



#endif