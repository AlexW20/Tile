#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include "entity.h"
using namespace std;

enum userAction { CreateEnemy, PlayerUp, PlayerDown, PlayerRight, PlayerLeft, PlayerHit };
enum BackgroundType { DayTime, Nighttime };

class World
{
private:
	vector<Entity*> entities;
	vector<Entity*> collisions;
	vector<Entity*> deletions;
	void checkCollision(Entity* entity, Entity* entity2);
	int worldLevel = 0;
	int deletedEnemiesCount = 0;
	bool stillAlive = true;

public:
	int level = 1;
	int coins = 0;
	World();
	void EnemyCount(int enemyBaseHealth);
	void clearWorld();
	void addEntity(int x, int y, int width, int height, EntityType type, string EntityName);
	void addStation(int x, int y, int width, int height, EntityType type, string EntityName, int price);
	void setWorldLevel(int worldLevel);
	int getWorldLevel();
	void addPlayer(int x, int y, int width, int height, EntityType type, string playerName, float health);
	void addEnemy(int x, int y, int width, int height, EntityType type, string playerName, float health);
	vector<Entity*> getWorldEntities();
	void tick();
	void increaseDeletedEnemies(int number);
	int getDeletedEnemiesCount();
	void setCoins(int coinNumber);
	int getCoins();
	bool getAliveState();
	void setAliveState(bool stillAlive);
};

#endif