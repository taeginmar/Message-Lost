#include "raylib.h"
#include "gameplay/entity/player.h"
#include "gameplay/entity/enemy.h"
#include <vector>

int main(){
    InitWindow(800, 600, "MESSAGE : LOST - Prototype Test");
    SetTargetFPS(60);

    Player player(100.0f, 100.0f, 32.0f, 32.0f, 100.0f, 200.0f, 100.0f);
    Enemy enemy(400.0f, 300.0f, 32.0f, 32.0f, 150.0f, 20.0f, 150.0f);

    std::vector<Rectangle> walls;
    walls.push_back(Rectangle{300.0f, 50.0f, 40.0f, 400.0f});
    walls.push_back(Rectangle{0.0f, 0.0f, 800.0f, 20.0f});

    while(!WindowShouldClose()){
        float dt = GetFrameTime();

        player.HandleMovement(dt, walls);
        enemy.Update(dt);

        BeginDrawing();
        ClearBackground(BLACK);

        for(const auto& wall : walls){
            DrawRectangleRec(wall, MAROON);
        }

        player.Draw();
        enemy.Draw();

        DrawText(TextFormat("Player HP: %.0f | Stamina: %.0f", player.GetHealth(), player.GetStamina()), 10, 30, 20, GREEN);
        DrawText(TextFormat("Player Speed: %.0f", player.GetCurrentSpeed()), 10, 50, 20, GREEN);
        DrawText("Use WASD to Move. Hold Shift to Run.", 10, 570, 16, LIGHTGRAY);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}