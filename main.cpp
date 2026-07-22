#include "raylib.h"
#include "levels/level.h"
#include "levels/Mock_Up_Level/mock_zones.h"
#include "gameplay/entity/player.h"
#include "core/game_state.h"

Player* gPlayer = nullptr;

void SetUpMockUpLevel(Level& level, Player& player);

int main() {
    InitWindow(800, 600, "Mock Up Game Test");
    SetTargetFPS(60);

    Player player(400, 300, 40, 40, 100.0f, 100.0f, 120.0f);
    player.SetSpawnPoint(400.0f, 300.0f);
    gPlayer = &player;

    Level* level = new Level();
    GameState gameState;

    SetUpMockUpLevel(*level, player);

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        if (gameState.GetState() == GameStateType::GAMEPLAY) {
            player.Update(dt);

            if (player.IsDead()) {
                gameState.SetState(GameStateType::GAME_OVER);
            }

            level->Update(player, dt, gameState);
        }

       if(gameState.GetState() == GameStateType::GAME_OVER && IsKeyPressed(KEY_ENTER)){
          delete level;

          level = new Level();
          player.ClearInventory();
          player.Respawn();

          SetUpMockUpLevel(*level, player);
          gameState.SetState(GameStateType::GAMEPLAY);
       }

        BeginDrawing();
        ClearBackground(GRAY);

        level->Draw();
        player.Draw();

        if (gameState.GetState() == GameStateType::GAME_OVER) {
            DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.75f));

            DrawText("GAME OVER", 210, 220, 60, RED);
            DrawText("Press ENTER to Restart", 140, 320, 40, WHITE);
        }

        EndDrawing();
    }

    delete level;

    CloseWindow();
    return 0;
}