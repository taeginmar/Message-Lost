#pragma once

#include "raylib.h"
#include "gameplay/item/consumable.h"
#include <vector>
#include <string>

class Inventory{
private:
    int capacity;
    std::vector<Item*> slots;
    bool isOpen;
    int selectedIndex;
    int columns;
    int draggedIndex;

    bool showContextMenu;
    int contextIndex;
    Vector2 contextMenuPos;
    int contextMenuSelected;

    std::string lastPickedItemType = "";
    float pickTimer = 0.0f;
    void DrawItemIcon(Item* item, float x, float y);
    void ExecuteContextAction(int action, int index);
public:
    Inventory(int size = 12, int cols = 4)
        : capacity(size), columns(cols), isOpen(false), selectedIndex(0),
          draggedIndex(-1), showContextMenu(false), contextMenuSelected(0) {
            slots.resize(capacity, nullptr);
          }
        
    ~Inventory() {}

    bool AddItem(Item* item);
    void RemoveItem(int index);
    void MoveItem(int fromIndex, int toIndex);
    void Toggle();
    bool IsOpen() const { return isOpen; }

    void Update(Player& player, std::vector<Item*>& items);
    void Draw(const Player& player);
};