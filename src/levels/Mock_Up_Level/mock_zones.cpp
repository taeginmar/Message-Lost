#include "mock_zones.h"

Door::Door(Rectangle b, int tz, Vector2 ep, Color c) 
    : bounds(b), targetZone(tz), exitPoint(ep), color(c) {}
void Door::Draw() { DrawRectangleRec(bounds, color); }

// --- MockZoneA Implementation ---
MockZoneA::MockZoneA() : Zone("Mock Zone A"), isNearDoor(false) {
    walls = {{50, 50, 700, 20}, {50, 530, 700, 20}};
    doors.push_back(Door({700, 200, 50, 100}, 1, {100, 300}));
    containers.push_back(Container({300, 300, 50, 50}));
}

void MockZoneA::Init() {}

void MockZoneA::Update(Player& player, int& nextZone, Vector2& spawnPos) {
    isNearDoor = false;
    for(auto& door : doors) {
        if(CheckCollisionRecs(player.GetBounds(), door.bounds)) {
            isNearDoor = true;
            if(IsKeyPressed(KEY_E)) {
                nextZone = door.targetZone;
                spawnPos = door.exitPoint;
            }
        }
    }
}

void MockZoneA::Draw() {
    ClearBackground(DARKGRAY);
    for(auto& w : walls) DrawRectangleRec(w, LIGHTGRAY);
    for(auto& door : doors) door.Draw();
    for(auto& c : containers) c.Draw(); // วาด Container Layer
    if(isNearDoor) DrawText("Interact (E)", 200, 50, 20, WHITE);
}

// --- MockZoneB Implementation ---
MockZoneB::MockZoneB() : Zone("Mock Zone B"), isNearDoor(false) {
    walls = {{200, 100, 400, 20}, {200, 400, 400, 20}};
    doors.push_back(Door({100, 300, 50, 100}, 0, {600, 300}));
}

void MockZoneB::Init() {}

void MockZoneB::Update(Player& player, int& nextZone, Vector2& spawnPos) {
    isNearDoor = false;
    for(auto& door : doors) {
        if(CheckCollisionRecs(player.GetBounds(), door.bounds)) {
            isNearDoor = true;
            if(IsKeyPressed(KEY_E)) {
                nextZone = door.targetZone;
                spawnPos = door.exitPoint;
            }
        }
    }
}

void MockZoneB::Draw() {
    ClearBackground(MAROON);
    for(auto& w : walls) DrawRectangleRec(w, GRAY);
    for(auto& door : doors) door.Draw();
    if(isNearDoor) DrawText("Interact (E)", 200, 50, 20, WHITE);
}

void SetUpMockUpLevel(Level& level) {
    level.AddZone(new MockZoneA());
    level.AddZone(new MockZoneB());
}