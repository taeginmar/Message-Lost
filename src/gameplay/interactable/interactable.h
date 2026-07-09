#pragma once

#include "raylib.h"
#include <string>

class Interactable{
protected:
    Rectangle bounds;
    std::string prompt;
    bool isActivated;
public:
    Interactable(float x, float y, float width, float height, std::string promptext)
    : bounds({x, y, width, height}), prompt(promptext), isActivated(false) {}

    virtual ~Interactable() {}

    virtual void OnInteract() = 0;

    Rectangle GetBounds() const { return bounds; }
    std::string GetPrompt() const { return prompt; }
    bool IsActivated() const { return isActivated; }

    virtual void Draw(){
        DrawRectangleRec(bounds, BLUE);
    }
};

//Testing Class Only
class Chest : public Interactable{
public:
    Chest(float x, float y) : Interactable(x, y, 40.0f, 40.0f, "Press E to Interact.") {}

    void OnInteract() override{
        if(!isActivated){
            isActivated = true;
            prompt = "Chest is Empty";
            TraceLog(LOG_INFO, "You found an item.");
        }
    }

    void Draw() override {
        Color chestColor = isActivated? GRAY : GOLD;
        DrawRectangleRec(bounds, chestColor);
    }
};