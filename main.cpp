#include "raylib.h"
#include "levels/level.h"
#include "levels/Mock_Up_Level/mock_zones.h"
#include "gameplay/entity/player.h"

void SetUpMockUpLevel(Level& level, Player& player);

int main(){
    InitWindow(800, 600, "Mock Up Game Test");
    SetTargetFPS(60);

   Player player(400, 300, 40, 40, 100.0f, 200.0f, 100.0f);
   Level level;

   SetUpMockUpLevel(level, player); //Chapter 0 : Mock Up Level

   while(!WindowShouldClose()){
        float dt = GetFrameTime();
        
        player.Update(dt);
        level.Update(player, dt);

        BeginDrawing();
        ClearBackground(GRAY);

        level.Draw();
        player.Draw();
        EndDrawing();
   }

   CloseWindow();
   return 0;
}
