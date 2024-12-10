#include <iostream>
#include <cstdlib>
#include <ctime>
#include "world.h"
#include "playerentity.h"
#include "enemyEntity.h"
#include "Upgrades/portalEntity.h"
#include "Upgrades/range.h"
#include "Upgrades/damage.h"
#include "Upgrades/health.h"
#include "Upgrades/movement.h"
#include "pubsub.h"
#include "raylib.h"

World::World()
{
	srand(time(NULL));
}

void World::EnemyCount(int enemyBaseHealth) {
	int enemyCount = 0;
	for (Entity* entity : entities) {
		if (entity->getType() == enemy) {
			enemyCount++;
		};

	}
	if (enemyCount == 0) {
		this->setWorldLevel(this->getWorldLevel() + 1);
		for (int i = 0; i < 5 * this->getWorldLevel(); i++) {
			int topBottomLeftOrRight = GetRandomValue(1, 4);
			switch (topBottomLeftOrRight) {
			case 1: 
				addEnemy(1 + (16 * scale), GetRandomValue(1 + (16 * scale), screenHeight - (16 * scale) - 1), 16, 16, enemy, "", enemyBaseHealth);
				break;
			case 2: 
				addEnemy(screenWidth - (16 * scale) - 1, GetRandomValue(1 + (16 * scale), screenHeight - (16 * scale) - 1), 16, 16, enemy, "", enemyBaseHealth);
				break;
			case 3: 
				addEnemy(GetRandomValue(1 + (16 * scale), screenWidth - (16 * scale) - 1), 1 + (16 * scale), 16, 16, enemy, "", enemyBaseHealth);
				break;
			case 4: 
				addEnemy(GetRandomValue(1 + (16 * scale), screenWidth - (16 * scale) - 1), screenHeight - (16 * scale) - 1, 16, 16, enemy, "", enemyBaseHealth);
				break;
			}
		};

		for (Entity* entity : entities) {
			if (entity->getType() == enemy) {
				entity->setMaxHealth(entity->getMaxHealth() + (1.5 * this->getWorldLevel()));
				entity->setHealth(entity->getMaxHealth());
				entity->setPower(entity->getPower() + (1.25 * this->getWorldLevel()));
			}
		}
	}

}

void World::tick()
{
	for (Entity* entity : entities) {
		entity->tick();
	}


	
	collisions.clear();
	for (Entity* entity : entities) {
		if (entity->getMoving() || (entity->getType() != guy && entity->getType() != enemy)) {
			for (Entity* entity2 : entities) {
				if (entity != entity2) {
					checkCollision(entity, entity2);
				}
			}
		}
	}


	
	deletions.clear();
	for (Entity* entity : collisions) {
		if (entity->handleCollision()) {
			deletions.push_back(entity);

		}
	}
	
	for (Entity* entity : entities) {
		if (entity->getHealth() <= 0) {
			deletions.push_back(entity);
		}
	}
	
	for (Entity* entity : deletions)
	{
	
		vector<Entity*>::iterator result = find(entities.begin(),
			entities.end(), entity);
		if (result != entities.end())
			entities.erase(result);
		
		PubSub::publish("entity", "delete", entity);
		delete entity;
		

	}
}

void World::increaseDeletedEnemies(int number)
{
	this->deletedEnemiesCount += number;
}

int World::getDeletedEnemiesCount()
{
	return this->deletedEnemiesCount;
}

void World::setCoins(int coinNumber)
{
	this->coins = coinNumber;
}
int World::getCoins() {
	return this->coins;
}
bool World::getAliveState()
{
	return stillAlive;
}
void World::setAliveState(bool stillAlive) {
	this->stillAlive = stillAlive;
}
void World::checkCollision(Entity* entity, Entity* entity2) {
	
	if ((entity->getHealth() <= 0 || entity2->getHealth() <= 0) && entity->getType() == enemy || entity2->getType() == enemy) {
		return; 
	}
	Rectangle rec1 = { entity->getXPos(),entity->getYPos(), entity->getWidth(), entity->getHeight() };
	Rectangle rec2 = { entity2->getXPos(),entity2->getYPos(), entity2->getWidth(), entity2->getHeight() };
	if (CheckCollisionRecs(rec1, rec2)) {
		entity->addCollision(entity);
		entity->addCollision(entity2);


		if (find(collisions.begin(), collisions.end(), entity) == collisions.end()) {
			collisions.push_back(entity);
			collisions.push_back(entity2);
		}
	}
}

void World::clearWorld()
{
	for (Entity* entity : entities)
	{
		
		PubSub::publish("entity", "delete", entity);
		delete entity;
	}

	entities.clear();
}

void World::addEntity(int x, int y, int width, int height, EntityType type, string EntityName)
{
	Entity* entity = new Entity(x, y, width, height, type, EntityName);

	
	PubSub::publish("entity", "new", entity);

	entities.push_back(entity);
}

void World::addStation(int x, int y, int width, int height, EntityType type, string stationName, int price)
{
	Entity* entity;
	switch (type) {
	case portal:
		entity = new PortalEntity(x, y, width, height, type, stationName, price);
		break;
	case range:
		entity = new Range(x, y, width, height, type, stationName, price);
		break;
	case health:
		entity = new Health(x, y, width, height, type, stationName, price);
		break;
	case damage:
		entity = new Damage(x, y, width, height, type, stationName, price);
		break;
	case movement:
		entity = new movement(x, y, width, height, type, stationName, price);
		break;
	}
	PubSub::publish("entity", "new", entity);
	entities.push_back(entity);
}

void World::setWorldLevel(int worldLevel)
{
	this->worldLevel = worldLevel;
}

int World::getWorldLevel()
{
	return this->worldLevel;
}


void World::addPlayer(int x, int y, int width, int height, EntityType type, string playerName, float health)
{
	Entity* entity = new PlayerEntity(x, y, width, height, type, playerName, health);

	
	PubSub::publish("entity", "new", entity);

	entities.push_back(entity);

}

void World::addEnemy(int x, int y, int width, int height, EntityType type, string playerName, float health)
{
	Entity* player;
	for (Entity* entity : entities) {
		if (entity->getType() == guy) {
			player = entity;
			break;
		}
	}

	Entity* entity = new EnemyEntity(x, y, width, height, type, playerName, health, player);

	
	PubSub::publish("entity", "new", entity);

	entities.push_back(entity);
}

vector<Entity*> World::getWorldEntities()
{
	return this->entities;
}

