#ifndef WORLDVIEW_H
#define WORLDVIEW_H

#include "raylib.h"
#include "world.h";

class WorldView
{
private:
	Texture2D background;
	int width, height;

public:
	WorldView();
	void draw(int coins, int worldLevel, int deletedEnemies);
	void deathScreenDraw(int worldLevel, int deletedEnemies);
};



#endif}