#include <map>
#include <string>
#include "gameView.h"
#include "textures.h"
#include "raylib.h"

struct SpriteSheetInfo
{
    string fileName;
    int numFrames = 0;
    map<EntityFacing, int> facings;
};
using namespace std;
map<EntityType, SpriteSheetInfo> spriteMapping = {
    {guy, {"", 4, {
        {right, 2},
        {up, 1},
        {down, 0},
        {left, 3}}
    }},
    {portal, {"", 4, {
        {right, 0}}
    }},

    
    {range, {"", 4, {
        {right, 0}}
    }},
   
    {health, {"", 4, {
        {right, 0}}
    }},
  
    {damage, {"", 4, {
        {right, 0}}
    }},
    
    {movement, {"", 4, {
        {right, 0}}
    }},
    {
   enemy, {"", 4, {
   {right, 2},
   {up, 1},
   {down, 0},
   {left, 3}}
}
}
};

EntityView::EntityView(Entity* entity)
{
    this->entity = entity;
    texture = Textures::loadTexture(spriteMapping[entity->getType()].fileName);
    frame = 0;
    delay = 0;
}

bool EntityView::isViewFor(Entity* entity)
{
    return this->entity == entity;
}

void EntityView::draw()
{
    int animationDelay = 20;
    int numFrames = spriteMapping[entity->getType()].numFrames;

    if (entity->getMoving())
    {
        delay--;

        if (delay <= 0)
        {
            frame = (frame + 1) % numFrames;
            delay = animationDelay;
        }
    }
    else if (entity->getType() != enemy && entity->getType() != jerry) {
        delay--;

        if (delay <= 0)
        {
            frame = (frame + 1) % numFrames;
            delay = animationDelay;
        }
    }
    else
    {
        frame = 0;
        delay = animationDelay;
    }

    int frameWidth = texture.width / numFrames;
    int row = spriteMapping[entity->getType()].facings[entity->getFacing()];

    
    Rectangle srcRec = {
        (frame * frameWidth),
        (row * frameWidth),
        frameWidth ,
        frameWidth };


    
    Rectangle destRec = {
        entity->getXPos(),
        entity->getYPos(),
        frameWidth * scale,    
        frameWidth * scale     
    };

    
    if (entity->getType() == guy) {
        PlayerEntity* player = (PlayerEntity*)entity;
        float circleX, circleY;
        switch (player->getFacing()) {
        case right:
            circleX = player->getWidth() + player->getXPos() + player->getAttackRange();
            circleY = player->getYPos() + (player->getHeight() / 2);
            break;
        case left:
            circleX = player->getXPos() - player->getAttackRange();
            circleY = player->getYPos() + (player->getHeight() / 2);
            break;
        case up:
            circleX = player->getXPos() + (player->getWidth() / 2);
            circleY = player->getYPos() - player->getAttackRange();;
            break;
        case down:
            circleX = player->getXPos() + (player->getWidth() / 2);
            circleY = player->getYPos() + player->getHeight() + player->getAttackRange();
            break;
        default:
            circleX = player->getWidth() + player->getXPos() + player->getAttackRange();
            circleY = player->getYPos() + (player->getHeight() / 2);
        }
        DrawCircle(circleX, circleY, 3, RED);

    }
   
    else if (entity->getLevel() != -1) {
        string nameAndLevel = entity->getEntityName();
        nameAndLevel += " " + to_string((entity->getLevel()));
        string cost = "Cost: $" + to_string((entity->getPrice()));
        int fontSize = 15;
        DrawText(nameAndLevel.c_str(), (entity->getXPos() + (entity->getWidth() / 2)) - fontSize * 2, entity->getYPos() + entity->getHeight() + 15, fontSize, BLACK);
        DrawText(cost.c_str(), (entity->getXPos() + (entity->getWidth() / 2)) - fontSize * 2, entity->getYPos() + entity->getHeight() + fontSize + 15, fontSize, BLACK);
    }

  
    DrawTexturePro(texture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
    
    if (hitbox) {
        DrawRectangleLines(entity->getXPos(), entity->getYPos(), entity->getWidth(), entity->getHeight(), GREEN);
    }
  
    float healthBarWidth = (entity->getWidth()) * (entity->getHealth() / entity->getMaxHealth());
    float healthBarHeight = 20;

    
    DrawRectangle(entity->getXPos(), entity->getYPos() - 20, healthBarWidth, 10, RED);

   
    DrawRectangleLines(entity->getXPos(), entity->getYPos() - 20, entity->getWidth(), 10, BLACK);


}