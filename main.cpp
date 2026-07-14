#include "raylib.h"
#include <vector>
#include "gameplay/entity/player.h"
#include "levels/level.h"
#include "gameplay/inventory/inventory.h"
#include "levels/Mock_Up_Level/mock_zones.h"

int main() {
    InitWindow(800, 600, "Test SandBox");
    Player player(400, 300, 30, 30, 100, 200, 100);
    std::vector<Item*> worldItems;
    
    Level testLevel;
    SetUpMockUpLevel(testLevel, player);

    while(!WindowShouldClose()){
        float dt = GetFrameTime();
        testLevel.Update(player);

        player.HandleMovement(dt, testLevel.GetCurrentZone()->GetWalls());
        player.GetInventory()->Update(player, worldItems);

        if(IsKeyPressed(KEY_TAB)){
            player.GetInventory()->Toggle();
        }

        BeginDrawing();
        testLevel.Draw();
        player.Draw();
        player.GetInventory()->Draw(player);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}