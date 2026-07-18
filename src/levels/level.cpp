#include "level.h"

Zone::~Zone(){
    for(auto container : containers) delete container;
    for(auto item: items) delete item;
    for(auto enemy : enemies) delete enemy;
}

void Zone::AddContainer(Container* container){
    containers.push_back(container);
}

void Zone::AddItems(Item* item){
    items.push_back(item);
}

void Zone::AddEnemy(Enemy* enemy){
    enemies.push_back(enemy);
}

void Zone::Update(Player& player, float dt){
    player.HandleMovement(dt, walls);
    for(auto* enemy : enemies) enemy->Update(dt, walls);
}

void Zone::Draw(){
    for (auto& w : walls) DrawRectangleRec(w, DARKGRAY);
    for (auto item : items) item->Draw();
    for (auto* enemy : enemies) enemy->Draw();

    for(auto* container : containers) container->Draw();
}

Level::~Level() {
    for(Zone* zone: zones) delete zone;
    zones.clear();
}

void Level::AddZone(Zone* zone) { zones.push_back(zone); }

void Level::AddDoor(Door door) {
    if(door.sourceZoneIndex == -1 && !zones.empty()){
        door.sourceZoneIndex = (int)zones.size() - 1;
    }
    doors.push_back(door);
}

void Level::Update(Player& player, float dt){
    if(IsKeyPressed(KEY_TAB) && player.GetInventory()) {
        player.GetInventory()->Toggle();
    }

    for(auto& door : doors) {
        if (door.sourceZoneIndex == currentZoneIndex) {
            if(CheckCollisionRecs(player.GetBounds(), door.bounds)) {
                if(IsKeyPressed(KEY_E)) {
                    if(door.targetZoneIndex != currentZoneIndex) {
                        int oldZoneIndex = currentZoneIndex;
                        currentZoneIndex = door.targetZoneIndex;
                        
                        // แก้ไขปัญหาที่ 2: ระบบค้นหาประตูปลายทางอัตโนมัติเพื่อกำหนดจุดเกิดใกล้ประตู
                        bool foundDestDoor = false;
                        for(const auto& destDoor : doors) {
                            if(destDoor.sourceZoneIndex == currentZoneIndex && destDoor.targetZoneIndex == oldZoneIndex) {
                                float spawnX = destDoor.bounds.x;
                                float spawnY = destDoor.bounds.y;
                                
                                // คำนวณตำแหน่งเกิดให้อยู่ใกล้ประตูฉากใหม่โดยไม่ทับฮิตบ็อกซ์ประตู
                                if(spawnX < 100) {
                                    spawnX += destDoor.bounds.width + 20; // ประตูอยู่ซ้าย เกิดเยื้องขวา
                                } else if(spawnX > 700) {
                                    spawnX -= (player.GetBounds().width + 20); // ประตูอยู่ขวา เกิดเยื้องซ้าย
                                } else if(spawnY < 100) {
                                    spawnY += destDoor.bounds.height + 20; // ประตูอยู่บน เกิดเยื้องล่าง
                                } else {
                                    spawnY -= (player.GetBounds().height + 20); // ประตูอยู่ล่าง เกิดเยื้องบน
                                }
                                
                                player.SetPosition(spawnX, spawnY);
                                foundDestDoor = true;
                                break;
                            }
                        }
                        
                        // หากไม่เจอประตูคู่ขนาน ให้ใช้ค่า exitPoint ตามที่ตั้งไว้เดิมเป็น Fallback
                        if(!foundDestDoor) {
                            player.SetPosition(door.exitPoint.x, door.exitPoint.y);
                        }
                    } 
                    else {
                        // ประตูประเภทกดแล้วรีเซ็ตตำแหน่งในฉากตัวเอง ให้ใช้ exitPoint เดิม
                        player.SetPosition(door.exitPoint.x, door.exitPoint.y);
                    }
                    return; 
                }
            }
        }
    }

    if(currentZoneIndex < (int)zones.size()) {
        zones[currentZoneIndex]->Update(player, dt);

        if(player.GetInventory()) {
            player.GetInventory()->Update(player, zones[currentZoneIndex]->items);
        }

        auto& currentItems = zones[currentZoneIndex]->items;
        for(size_t i = 0; i < currentItems.size(); i++) {
            if(CheckCollisionRecs(player.GetBounds(), currentItems[i]->GetBounds())) {
                if(IsKeyPressed(KEY_E)) {
                    player.GetInventory()->AddItem(currentItems[i]);
                    currentItems.erase(currentItems.begin() + i);
                    i--;
                }
            }
        }

        for(auto* c : zones[currentZoneIndex]->containers) {
            if(c->IsPlayerNear(player.GetBounds()) && IsKeyPressed(KEY_E)) {
                player.GetInventory()->OpenContainer(c);
            }
        }
    }
}

void Level::Draw(){
    if(currentZoneIndex < (int)zones.size()) zones[currentZoneIndex]->Draw();
    
    for(auto& door : doors){
        if(door.sourceZoneIndex == currentZoneIndex){
            door.Draw();
        }
    }
}