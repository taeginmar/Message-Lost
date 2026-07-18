#pragma once
#include "raylib.h"
#include <vector>
#include <string>
#include "gameplay/entity/player.h"
#include "gameplay/entity/enemy.h"
#include "gameplay/item/container.h"
#include "gameplay/item/item.h"
#include "gameplay/inventory/inventory.h"

class Door{
public:
    Rectangle bounds;
    int targetZoneIndex;
    Vector2 exitPoint;
    int sourceZoneIndex;
    
    Door(Rectangle b, int tz, Vector2 ep, int sz = -1): 
        bounds(b), targetZoneIndex(tz), exitPoint(ep), sourceZoneIndex(sz) {}
    void Draw() { DrawRectangleRec(bounds, YELLOW); }
};

class Zone{
public:
    std::string ZoneName;
    std::vector<Rectangle> walls;

    std::vector<Container*> containers;
    std::vector<Item*> items;
    std::vector<Enemy*> enemies;

    Zone(std::string name) : ZoneName(name) {}
    ~Zone();

    void AddContainer(Container* container);
    void AddItems(Item* item);
    void AddEnemy(Enemy* enemy);

    void Update(Player& player, float dt);

    void Draw();
};

class Level{
private:
    std::vector<Zone*> zones;
    std::vector<Door> doors;
    int currentZoneIndex = 0;

public:
    Level() = default;
    ~Level();

    void AddZone(Zone* zone);
    void AddDoor(Door door);

    void Update(Player& player, float dt);
    void Draw();
};