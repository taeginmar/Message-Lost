#pragma once

#include "raylib.h"
#include "gameplay/item/consumable.h"
#include <vector>
#include <string>

struct InventorySlot{
    Item* itemDetails = nullptr;
    int count = 0;
};

class Container{
    std::vector<Item*> items;
public:
    const std::vector<Item*>& GetItems() const { return items; }
    void RemoveItem(int index) { items.erase(items.begin() + index); }
    void AddItem(Item* item) { items.push_back(item); }
};

class InventoryContextMenu{
public:
    bool show;
    int index;
    Vector2 pos;
    int selectedOpt;
    std::string option[3];

    InventoryContextMenu() : show(false), index(-1), pos{0,0},selectedOpt(0) {
        option[0] = "USE", option[1] = "INSPECT", option[2] = "REMOVE";
    }

    void Update(Vector2 mousePos);
    void Draw();
};

class Inventory{
private:
    int capacity;
    std::vector<InventorySlot> slots;
    bool isOpen;
    int selectedIndex;
    int columns;
    int draggedIndex;

    //Visual Feedback for Pick Item
    std::string lastPickedItemType;
    float pickTimer;

    InventoryContextMenu contextMenu;

    Container* linkedContainer = nullptr;
    bool isLootingMode = false;

    void DrawDualGrid(float startX, float startY);

    void HandleItemInteraction(Player& player, std::vector<Item*>& worldItems);
    void HandleSlotHover(Vector2 mousePos, float startX, float startY);
    void HandleContextMenuTrigger(Vector2 mousePos, float startX, float startY);
    void HandleDragAndDrop(Vector2 mousePos, float startX, float startY);
    void ExecuteAction(int action, int index, Player& player, std::vector<Item*>& worldItems);
    void DrawSlotGrid(float startX, float startY);

public:
    Inventory(int size = 12, int cols = 4);
    ~Inventory();

    void OpenLootMode(Container* container);
    void CloseLootMode();

    bool AddItem(Item* item);
    void RemoveItem(int index);
    void MoveItem(int fromIndex, int toIndex);
    void Toggle();
    bool IsOpen() const;

    void Update(Player& player, std::vector<Item*>& worldItems);
    void Draw(const Player& player);
};