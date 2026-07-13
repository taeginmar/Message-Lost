#pragma once
#include "raylib.h"
#include "gameplay/entity/player.h"
#include <vector>
#include <string>

class Player; 

class Zone{
protected:
    std::vector<Rectangle> walls;
    std::string ZoneName;
public:
    Zone(std::string name) : ZoneName(name) {}
    virtual ~Zone() {}

    virtual void Init() = 0;
    virtual void Update(Player& player, int& nextZone, Vector2& spawnPos) = 0;
    virtual void Draw() = 0;

    std::string GetName() const { return ZoneName; }
    const std::vector<Rectangle>& GetWalls() const { return walls; }
};

class Level{
private:
    std::vector<Zone*> zones;
    int currentZoneIndex;
public:
    Level();
    ~Level();

    void AddZone(Zone* zone);
    void SwitchZone(int index, Player& player, Vector2 spawnPos);

    void Update(Player& player);
    void Draw();

    Zone* GetCurrentZone() const;
};
