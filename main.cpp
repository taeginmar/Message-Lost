#include "raylib.h"
#include "gameplay/entity/player.h"
#include "gameplay/entity/enemy.h"
#include "gameplay/interactable/interactable.h"
#include "gameplay/item/consumable.h"
#include "gameplay/inventory/inventory.h"
#include <vector>

float uiTextTimer = 0.0f; 

int main(){
    // =========================================================================
    // INITIALIZATION & OBJECT SETUP (การกำหนด Object เริ่มต้น)
    // =========================================================================
    InitWindow(800, 600, "MESSAGE : LOST - Prototype Test");
    SetTargetFPS(60);

    Player player(100.0f, 100.0f, 32.0f, 32.0f, 100.0f, 100.0f, 100.0f);

    Enemy* enemy_1 = new Enemy(400.0f, 300.0f, 32.0f, 32.0f, 100.0f, 20.0f, 0.0f, &player, 
                            Enemy::NormalAction::PATROL, Enemy::PatrolType::HORIZONTAL, 1, 150.0f, 10.0f);
    Enemy* enemy_2 = new Enemy(750.0f, 200.0f, 32.0f, 32.0f, 100.0f, 20.0f, 0.0f, &player, 
                            Enemy::NormalAction::PATROL, Enemy::PatrolType::VERTICAL, 1, 100.0f, 50.0f);

    std::vector<Enemy*> enemies;
    enemies.push_back(enemy_1);
    enemies.push_back(enemy_2);

    std::vector<Rectangle> walls;
    walls.push_back(Rectangle{300.0f, 50.0f, 40.0f, 400.0f});
    walls.push_back(Rectangle{0.0f, 0.0f, 800.0f, 20.0f});

    Chest secretChest(500.0f, 400.0f);

    Medkit medKit_1(150.0f, 450.0f, player);
    Medkit medKit_2(700.0f, 100.0f, player);
    EnergyDrink drink_1(200.0f, 250.0f, player);
    EnergyDrink drink_2(600.0f, 500.0f, player);

    std::vector<Item*> items;
    items.push_back(&medKit_1);
    items.push_back(&medKit_2);
    items.push_back(&drink_1);
    items.push_back(&drink_2);

    Inventory inventory(12, 4); 
    Interactable* closestInteractable = nullptr;

    // =========================================================================
    // GAME LOOP
    // =========================================================================
    while(!WindowShouldClose()){
        // ---------------------------------------------------------------------
        // UPDATE LOGIC (การนำ Object มาใช้ใน Loop)
        // ---------------------------------------------------------------------
        float dt = GetFrameTime();
        closestInteractable = nullptr;

        if (!inventory.IsOpen()) {
            player.HandleMovement(dt, walls);
        }

        for(auto enemy : enemies){
            enemy->Update(dt, walls);
        }

        static float lastPlayerHP = player.GetHealth();
        float currentHP = player.GetHealth();
        if(currentHP < lastPlayerHP){
            uiTextTimer = 1.5f;
        }
        lastPlayerHP = currentHP;

        if(uiTextTimer > 0.0f){ 
            uiTextTimer -= dt;
        }

        if (IsKeyPressed(KEY_TAB)) {
            inventory.Toggle();
        }

        inventory.Update(player, items);

        if (!secretChest.IsActivated() && CheckCollisionRecs(player.GetBounds(), secretChest.GetBounds())) {
            closestInteractable = &secretChest; 
            if (IsKeyPressed(KEY_E)) {
                closestInteractable->OnInteract();
                uiTextTimer = 1.5f; 
            }
        }

        // ---------------------------------------------------------------------
        // RENDER / DRAWING (การใช้คำสั่งแสดงภาพ)
        // ---------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(BLACK);

        for(const auto& wall : walls){
            DrawRectangleRec(wall, MAROON);
        }

        player.Draw();
        for(auto enemy : enemies){
            enemy->Draw();
        }

        secretChest.Draw();

        for (auto item : items) {
            item->Draw();
        }

        if(closestInteractable != nullptr){
            DrawText(closestInteractable->GetPrompt().c_str(), 300, 500, 20, YELLOW);
        }

        DrawText(TextFormat("Player HP: %.0f | Stamina: %.0f", player.GetHealth(), player.GetStamina()), 10, 30, 20, GREEN);
        DrawText(TextFormat("Player Speed: %.0f", player.GetCurrentSpeed()), 10, 50, 20, GREEN);
        DrawText("Use W A S D to Move. Hold LShift to Run. Press TAB for Inventory.", 10, 570, 16, LIGHTGRAY);

        if(player.GetHealth() <= 0){
            DrawText("Player is Dead.", 400, 300, 30, RED);
        }
        else if(uiTextTimer > 0.0f && currentHP < lastPlayerHP){
            DrawText("Player is Attacked.", 400, 300, 30, RED);
        }

        inventory.Draw(player);
    
        EndDrawing();
    }

    // =========================================================================
    // CLEANUP & SHUTDOWN
    // =========================================================================
    for(auto enemy : enemies){
        delete enemy;
    }
    CloseWindow();
    return 0;
}