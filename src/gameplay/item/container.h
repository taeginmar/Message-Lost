#pragma once
#include <vector>
#include "gameplay/item/consumable.h"

class Container{
private:
    Rectangle bounds;
    std::vector<Item*> items;
public:
    Container(Rectangle b) : bounds(b) {}

    bool IsPlayerNear(Rectangle playerBounds) const { 
        return CheckCollisionRecs(playerBounds, bounds);
    }

    void AddItem(Item* item) { items.push_back(item);}
    std::vector<Item*>& GetItems() { return items; }
    void RemoveItem(int index) { items.erase(items.begin() + index); }
    Rectangle GetBounds() { return bounds; }

    void Draw(){
        DrawRectangleRec(bounds, BROWN);
        DrawText("BOX", bounds.x + 10, bounds.y + 10, 10, WHITE);
    }
};