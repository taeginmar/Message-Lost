#pragma once
#include "raylib.h"
#include <vector>
#include <string>
#include "core/ui_manager.h"
#include "gameplay/entity/player.h"
#include "gameplay/item/container.h"
#include "gameplay/item/item.h"
#include "core/objective_manager.h"

extern ObjectiveManager gObjectiveManager;
class Item; class Container; class Player;

struct InventorySlot {
    Item* itemDetails = nullptr;
    int count = 0;
    bool isLocked = false;
};

class InventoryContextMenu {
public:
    bool show = false;
    int index = -1;
    int selectedOpt = 0;
    Vector2 pos = { 0, 0 };
    std::string option[3] = { "Use", "Inspect", "Drop" };
    void Update(Vector2 mousePos);
    void Draw();
};

class Inventory {
private:
    int capacity; 
    int columns; 
    int selectedIndex; 
    int draggedIndex;
    bool isOpen; 
    bool isLootingMode; 
    float pickTimer;

    static constexpr int MAX_STACK = 3;

    Container* linkedContainer;
    std::vector<InventorySlot> slots;
    InventoryContextMenu contextMenu;
    float stackLimitWarningTimer = 0.0f;
    int stackLimitWarningSlot = -1;

    void HandleItemInteraction(Player& player, std::vector<Item*>& worldItems);
    void HandleSlotHover(Vector2 mousePos, float startX, float startY);
    void HandleContextMenuTrigger(Vector2 mousePos, float startX, float startY);
    void HandleDragAndDrop(Vector2 mousePos, float startX, float startY);

public:
    Inventory(int size, int cols);
    ~Inventory();
    bool AddItem(Item* item);
    void RemoveItem(int index);
    void MoveItem(int fromIndex, int toIndex);
    void OpenContainer(Container* container);
    void Toggle();
    bool IsOpen() const;
    void Update(Player& player, std::vector<Item*>& worldItems);
    void ExecuteAction(int action, int index, Player& player, std::vector<Item*>& worldItems);
    void Draw(const Player& player);
    void UnlockSlot(int index);
    void UnlockSlots(int count);
    void Clear();

    int GetItemCount(const std::string& targetItemType) const;
};