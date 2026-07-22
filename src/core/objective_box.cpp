#include "core/objective_box.h"

ObjectiveBox::ObjectiveBox(float x, float y, float width, float height)
    : bounds{x, y, width, height} {}

void ObjectiveBox::SetPosition(float x, float y) {
    bounds.x = x;
    bounds.y = y;
}

void ObjectiveBox::Update() {
}

void ObjectiveBox::Draw() {
    Objective* currentObj = gObjectiveManager.GetCurrentObjective();

    DrawRectangleRec(bounds, Fade(BLACK, 0.7f));
    DrawRectangleLinesEx(bounds, 2.0f, GOLD);

    if (currentObj != nullptr) {
        DrawText(currentObj->GetTitle().c_str(), (int)bounds.x + 15, (int)bounds.y + 15, 18, YELLOW);
        DrawText(currentObj->GetDescription().c_str(), (int)bounds.x + 15, (int)bounds.y + 42, 14, RAYWHITE);
    } else {
        DrawText("No Active Objective", (int)bounds.x + 15, (int)bounds.y + 15, 18, GRAY);
    }
}