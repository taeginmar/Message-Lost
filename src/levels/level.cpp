#include "level.h"

Zone::~Zone(){
    for(auto container : containers) delete container;
    for(auto item: items) delete item;
    for(auto enemy : enemies) delete enemy;
    for(auto object : interactables) delete object;

    containers.clear();
    items.clear();
    enemies.clear();
    interactables.clear();
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

void Zone::AddInteractable(Interactable* object){
    if(object == nullptr) return;
    
    interactables.push_back(object);
}

void Zone::Update(Player& player, float dt){
    if(player.IsDead()) return;
    player.HandleMovement(dt, walls);
    for(auto* enemy : enemies) enemy->Update(dt, walls);

    Rectangle playerBounds = player.GetBounds();

    for(auto* object : interactables){
        if(object == nullptr) continue;

        if(CheckCollisionRecs(playerBounds,object->GetBounds())){
            if(IsKeyPressed(KEY_E)){
                 object->OnInteract();
            }
        }
    }
}

void Zone::Draw(){
    for (auto& w : walls) DrawRectangleRec(w, DARKGRAY);
    for (auto item : items) item->Draw();
    for (auto* enemy : enemies) enemy->Draw();

    for(auto* object : interactables) object->Draw();
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

void Level::Update(Player& player, float dt, GameState& gameState){
    if(!gameState.CanUpdateWorld()) return;
    
    if (gameState.CanOpenInventory() && IsKeyPressed(KEY_TAB) && player.GetInventory()) {
        player.GetInventory()->Toggle();
    }

    for (auto& door : doors) {
        if (door.sourceZoneIndex == currentZoneIndex) {
            if (CheckCollisionRecs(player.GetBounds(), door.bounds)) {
                if (IsKeyPressed(KEY_E)) {

                    if (door.targetZoneIndex != currentZoneIndex) {
                        int oldZoneIndex = currentZoneIndex;
                        currentZoneIndex = door.targetZoneIndex;
                        bool foundDestDoor = false;

                        for (const auto& destDoor : doors) {
                            if (destDoor.sourceZoneIndex == currentZoneIndex &&
                                destDoor.targetZoneIndex == oldZoneIndex) {

                                float spawnX = destDoor.bounds.x;
                                float spawnY = destDoor.bounds.y;

                                if (spawnX < 100) {
                                    spawnX += destDoor.bounds.width + 20;
                                }
                                else if (spawnX > 700) {
                                    spawnX -= (player.GetBounds().width + 20);
                                }
                                else if (spawnY < 100) {
                                    spawnY += destDoor.bounds.height + 20;
                                }
                                else {
                                    spawnY -= (player.GetBounds().height + 20);
                                }

                                player.SetPosition(spawnX, spawnY);
                                foundDestDoor = true;
                                break;
                            }
                        }

                        if (!foundDestDoor) {
                            player.SetPosition(
                                door.exitPoint.x,
                                door.exitPoint.y
                            );
                        }
                    }
                    else {
                        player.SetPosition(
                            door.exitPoint.x,
                            door.exitPoint.y
                        );
                    }

                    return;
                }
            }
        }
    }

    if(currentZoneIndex >= (int)zones.size()) { return; }

    Zone* currentZone = zones[currentZoneIndex];
    currentZone->Update(player, dt);

    if(gObjectiveManager.GetCurrentObjective()==nullptr){
        gameState.SetState(GameStateType::MOCK_COMPLETE);
        return;
    }
    
    player.Attack(currentZone->enemies);

    if(player.GetInventory()){
        player.GetInventory()->Update(player,currentZone->items);

        int currentMedKitCount = player.GetInventory()->GetItemCount("MedKit");

        gObjectiveManager.UpdateProgress( ObjectiveType::COLLECT_ITEM,"medkit", currentMedKitCount);
    }

    for(auto* c : currentZone->containers){
        if(c->IsPlayerNear(player.GetBounds()) && IsKeyPressed(KEY_E)){
            player.GetInventory()->OpenContainer(c);
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