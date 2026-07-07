#pragma once
#include "raylib.h"

class GameObject{
protected:
    Rectangle bounds;
    bool isActive;

public:
    GameObject(float x, float y, float width, float height);
    virtual ~GameObject();

    virtual void Update(float dt) = 0;
    virtual void Draw() = 0;

    Rectangle GetBounds() const {return bounds;}
    void SetPosition(float x, float y) {bounds.x = x; bounds.y = y;}
    Vector2 GetPosition() const {return Vector2{bounds.x, bounds.y};}

    bool IsActive() const {return isActive; }
    void SetActive(bool active) { isActive = active; }
};