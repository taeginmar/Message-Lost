#include "raylib.h"
#include "gameplay/entity/player.h"
#include "gameplay/entity/enemy.h"
#include <vector>

float uiTextTimer = 0.0f; 

int main(){
    InitWindow(800, 600, "MESSAGE : LOST - Prototype Test");
    SetTargetFPS(60);

    //float x, float y, float width, float height, float health, float speed, float stamina
    Player player(100.0f, 100.0f, 32.0f, 32.0f, 100.0f, 100.0f, 100.0f);

    //float x, float y, float width, float height, float health, float speed, float sensing_bar
    // 🎯 ศัตรูตัวที่ 1: เดินลาดตระเวน แนวนอน (HORIZONTAL) ซ้าย-ขวา
    Enemy* enemy_1 = new Enemy(400.0f, 300.0f, 32.0f, 32.0f, 100.0f, 20.0f, 0.0f, &player, 
                            Enemy::NormalAction::PATROL, 
                            Enemy::PatrolType::HORIZONTAL, 1, 150.0f, 10.0f);

    // 🎯 ศัตรูตัวที่ 2: เดินลาดตระเวน แนวตั้ง (VERTICAL) ขึ้น-ลง เฝ้าทางเดิน
    Enemy* enemy_2 = new Enemy(800.0f, 200.0f, 32.0f, 32.0f, 100.0f, 20.0f, 0.0f, &player, 
                            Enemy::NormalAction::PATROL, 
                            Enemy::PatrolType::VERTICAL, 1, 100.0f, 50.0f);

    std::vector<Enemy*> enemies;
    enemies.push_back(enemy_1);
    enemies.push_back(enemy_2);

    std::vector<Rectangle> walls;
    walls.push_back(Rectangle{300.0f, 50.0f, 40.0f, 400.0f});
    walls.push_back(Rectangle{0.0f, 0.0f, 800.0f, 20.0f});

    while(!WindowShouldClose()){
        float dt = GetFrameTime();

        player.HandleMovement(dt, walls);
        for(auto enemy : enemies){
            enemy->Update(dt, walls);
        }

        static float lastPlayerHP = player.GetHealth();
        float currentHP = player.GetHealth();

        if(currentHP < lastPlayerHP){
            uiTextTimer = 1.5f;
        }

        lastPlayerHP = currentHP;

        if(uiTextTimer > 0.0f){ // ตัวนับเวลาถอยหลังของข้อความ > 0.0f){
            uiTextTimer -= dt;
        }

        BeginDrawing();
        ClearBackground(BLACK);

        for(const auto& wall : walls){
            DrawRectangleRec(wall, MAROON);
        }

        player.Draw();
        for(auto enemy : enemies){
            enemy->Draw();
        }

        DrawText(TextFormat("Player HP: %.0f | Stamina: %.0f", player.GetHealth(), player.GetStamina()), 10, 30, 20, GREEN);
        DrawText(TextFormat("Player Speed: %.0f", player.GetCurrentSpeed()), 10, 50, 20, GREEN);
        DrawText("Use W A S D to Move. Hold LShift to Run.", 10, 570, 16, LIGHTGRAY);

        if(player.GetHealth() <= 0){
            DrawText("Player is Dead.", 400, 300, 30, RED);
        }
        else if(uiTextTimer > 0.0f){
            DrawText("Player is Attacked.", 400, 300, 30, RED);
        }
    
        EndDrawing();
    }

    for(auto enemy : enemies){
        delete enemy;
    }
    CloseWindow();
    return 0;
}