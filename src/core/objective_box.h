#pragma once
#include "core/objective_manager.h"
#include "raylib.h"

class ObjectiveBox {
public:
    ObjectiveBox(float x = 0.0f, float y = 0.0f, float width = 300.0f, float height = 100.0f);

    void SetPosition(float x, float y);
    void Update();
    void Draw();

private:
    Rectangle bounds;
};