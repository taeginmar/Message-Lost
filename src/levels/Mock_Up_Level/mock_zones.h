#pragma once
#include "levels/level.h"
#include "gameplay/item/container.h"
#include "gameplay/item/item.h"
#include <vector>

class Door {
public:
    Rectangle bounds;
    int targetZone;
    Vector2 exitPoint;
    Color color;

    Door(Rectangle b, int tz, Vector2 ep, Color c = YELLOW);
    void Draw();
};

class MockZoneA : public Zone {
private:
    std::vector<Door> doors;
    std::vector<Container> containers;
    bool isNearDoor;
public:
    MockZoneA(Player& player);

    void Init() override;
    void Update(Player& player, int& nextZone, Vector2& spawnPos) override;
    void Draw() override;
};

class MockZoneB : public Zone {
private:
    std::vector<Door> doors;
    std::vector<Container> containers;
    bool isNearDoor;
public:
    MockZoneB(Player& player);
    
    void Init() override;
    void Update(Player& player, int& nextZone, Vector2& spawnPos) override;
    void Draw() override;
};

void SetUpMockUpLevel(Level& level, Player& player);