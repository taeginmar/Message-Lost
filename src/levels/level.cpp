#include "level.h"

Level::Level() : currentZoneIndex(0) {}

Level::~Level(){
    for(Zone* zone: zones){
        delete zone;
    }
    zones.clear();
}

void Level::AddZone(Zone* zone){
    zone->Init();
    zones.push_back(zone);
}

void Level::SwitchZone(int index, Player& player, Vector2 spawnPos){
    if(index >= 0 && index < zones.size()) {
        currentZoneIndex = index;
        player.SetPosition(spawnPos.x, spawnPos.y);
        TraceLog(LOG_INFO, "LOADING....", zones[currentZoneIndex]->GetName().c_str());
    }
}

void Level::Update(Player& player){
    int nextZone = -1;
    Vector2 spawnPos = {0, 0};

    zones[currentZoneIndex]->Update(player, nextZone, spawnPos);
    if(nextZone != -1) SwitchZone(nextZone, player, spawnPos);
}

void Level::Draw(){
    if(zones.empty()) return;
    zones[currentZoneIndex]->Draw();
}

Zone* Level::GetCurrentZone() const {
    if(zones.empty()) return nullptr;
    return zones[currentZoneIndex];
}