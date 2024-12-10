#include "raylib.h"
#include "controller.h"

#include <math.h>

int main(void)
{

    Controller c;
    c.gameLoop();

    return 0;
}
void Controller::gameLoop() {

    SetTargetFPS(60);
    map<KeyboardKey, userAction> keyMapping;

    keyMapping[KEY_W] = PlayerUp;
    keyMapping[KEY_A] = PlayerLeft;
    keyMapping[KEY_S] = PlayerDown;
    keyMapping[KEY_D] = PlayerRight;


    PubSub::subscribe("entity", this);

    InitWindow(GetScreenWidth(), GetScreenHeight(), "JerryRun");
    SetTargetFPS(60);
    ToggleFullscreen();
    screenWidth = GetScreenWidth();
    screenHeight = GetScreenHeight();

    // Load the level

    float x = 320;
    float y = 320;
    WorldView worldView;

    world.addStation(100, 10, 16, 16, portal, "Portal", 10); 
    world.addStation(300, 10, 16, 16, range, "Range", 15); 
    world.addStation(500, 10, 16, 16, health, "Health", 10); 
    world.addStation(700, 10, 16, 16, damage, "Damage", 10);
    world.addStation(900, 10, 16, 16, movement, "movement", 10);
    world.addPlayer(screenWidth / 2, screenHeight / 2, 16, 16, guy, "Guy", 40);

    while (!WindowShouldClose() && world.getAliveState())
    {
        world.EnemyCount(10);
        worldView.draw(world.getCoins(), world.getWorldLevel(), world.getDeletedEnemiesCount());


        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            userAction action = PlayerHit;
            PubSub::publish("action", "player", &action);
        }
        
        for (auto& i : keyMapping)
        {
            if (IsKeyDown(i.first))
            {
                userAction action = i.second;
                PubSub::publish("action", "player", &action);
            }
        }


        
        world.tick();

        BeginDrawing();

        ClearBackground(RAYWHITE);

       
        for (EntityView* view : views)
            view->draw();

        EndDrawing();


    }
    while (!WindowShouldClose() && world.getAliveState() == false) {


        worldView.deathScreenDraw(world.getWorldLevel(), world.getDeletedEnemiesCount());
        BeginDrawing();

        ClearBackground(RAYWHITE);
        EndDrawing();
    }
    world.clearWorld();


    CloseWindow();
}

void Controller::receiveMessage(string channel, string message, void* data)
{

    if (channel == "entity" && message == "hit") {
        Entity* entity = (Entity*)data;
        entity->setHealth(entity->getHealth() - (2 * world.getWorldLevel()));
    }
    
    if (channel == "entity" && message == "buyUpgrade") {
        Entity* station = (Entity*)data;
        if (station->getPrice() <= world.getCoins()) {
            world.setCoins(world.getCoins() - station->getPrice()); 
            station->setLevel(station->getLevel() + 1); 
            station->setPrice(station->getPrice() * 1.5); 
            PubSub::publish("Upgrade", "bought", upgrade);
        }
    }
    if (channel == "entity" && message == "new")
    {
       
        EntityView* view = new EntityView((Entity*)data);
        views.push_back(view);
    }

    if (channel == "entity" && message == "delete")
    {




     
        for (int i = 0; i < views.size(); i++)
        {


            if (views[i]->isViewFor((Entity*)data))
            {
                delete views[i];
               
                views.erase(views.begin() + i);
               
                break;
            }
        }

        Entity* entity = (Entity*)data;
        if (entity->getType() == guy) {
            world.setAliveState(false);
            return;
        }

        world.setCoins(world.getCoins() + GetRandomValue(3, 5));

        world.increaseDeletedEnemies(1);
    }
}