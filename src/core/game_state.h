#pragma once
#include "raylib.h"

enum class GameStateType {
    GAMEPLAY,
    INVENTORY,
    PAUSE,
    DIALOGUE,
    CUTSCENE,
    GAME_OVER,
    MOCK_COMPLETE
};

class GameState
{
private:
    GameStateType currentState = GameStateType::GAMEPLAY;

public:

    void SetState(GameStateType state) { currentState = state; }
    GameStateType GetState() const { return currentState; }

    bool CanPlayerMove() const { return currentState == GameStateType::GAMEPLAY; }
    bool CanEnemyUpdate() const { return currentState == GameStateType::GAMEPLAY; }
    bool CanInteract() const { return currentState == GameStateType::GAMEPLAY; }
    bool CanOpenInventory() const { return currentState == GameStateType::GAMEPLAY; }
    bool CanUpdateWorld() const { return currentState == GameStateType::GAMEPLAY; }
};