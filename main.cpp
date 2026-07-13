#include "raylib.h"
#include "gameplay/entity/player.h"
#include "levels/level.h"
#include "levels/Mock_Up_Level/mock_zones.h"

int main() {
    InitWindow(800, 600, "Test SandBox");
    Player player(400, 300, 30, 30, 100, 200, 100);
    
    Level testLevel;
    SetUpMockUpLevel(testLevel);

    while(!WindowShouldClose()){
        float dt = GetFrameTime();
        testLevel.Update(player);

        player.HandleMovement(dt, testLevel.GetCurrentZone()->GetWalls());

        BeginDrawing();
        testLevel.Draw();
        player.Draw();
        EndDrawing();
    }
    CloseWindow();
    return 0;
}