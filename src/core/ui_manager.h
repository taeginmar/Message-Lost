#pragma once

#include "raylib.h"
#include <vector>
#include <string>
#include <cmath>

#include "gameplay/item/item.h"

struct ContainerDisplaySlot {
    void* sampleItem;
    int count;
    int firstRealIndex;
};

struct UIRenderSlot {
    bool isLocked;
    void* itemPtr;
    int count;
    bool isDragged;
};

struct InventoryUIData {
    int capacity;
    int columns;

    int selectedIndex;
    int draggedIndex;

    bool isOpen;
    bool isLootingMode;

    float hp;
    float maxHp;

    float stamina;
    float maxStamina;

    std::string objective;

    std::vector<UIRenderSlot> slots;
    std::vector<UIRenderSlot> containerSlots;
};

class Player;

class UIManager {
public:
    static void DrawInventoryUI(const InventoryUIData& data, const Player& player);
    static void DrawSlotGrid(
        const std::vector<UIRenderSlot>& slots,
        int capacity,
        int columns,
        int selectedIndex,
        int draggedIndex,
        float startX,
        float startY,
        bool isContainer
    );

private:
    static void DrawNotebookBackground(float panelX, float panelY, float width, float height);
    static void DrawProfileSection(const Player& player, float x, float y);
    static void DrawInventorySection(const InventoryUIData& data, float x, float y);
    static void DrawContainerSection(const InventoryUIData& data, float x, float y);

    static void DrawStatBar(const char* label, float value, float maxValue, float x, float y, Color fillColor );
};