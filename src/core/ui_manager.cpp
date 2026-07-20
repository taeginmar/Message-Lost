#include "ui_manager.h"

const float SLOT_SIZE = 50.0f;

void UIManager::DrawNotebookBackground(float panelX, float panelY, float width, float height) {
    DrawRectangle(panelX, panelY, width, height, Fade(DARKGRAY, 0.7f));
    DrawRectangleLinesEx({ panelX, panelY, width, height }, 2, DARKGRAY);

    DrawLineEx({ panelX + width / 2.0f, panelY },{ panelX + width / 2.0f, panelY + height },4 ,DARKGRAY);
}

void UIManager::DrawStatBar(
    const char* label,
    float value,
    float maxValue,
    float x,
    float y,
    Color fillColor
) {
    DrawText(label, x, y, 16, LIGHTGRAY);

    DrawRectangle(x, y + 20, 150, 15, BLACK);

    float percent = 0.0f;

    if (maxValue > 0.0f) {
        percent = value / maxValue;
    }

    if (percent < 0.0f) percent = 0.0f;
    if (percent > 1.0f) percent = 1.0f;

    DrawRectangle(x, y + 20, 150 * percent, 15, fillColor);
}

void UIManager::DrawProfileSection(const Player& player, float x, float y) {
    DrawRectangle(x, y, 100, 100, Fade(BLACK, 0.35f));
    DrawRectangleLinesEx({x, y, 100, 100}, 2, LIGHTGRAY);

    DrawText("PROFILE", x + 20, y + 42, 12, LIGHTGRAY);

    DrawStatBar("HP", player.GetHealth(), 100.0f, x + 120, y, RED);
    DrawStatBar("STAMINA", player.GetStamina(), 100.0f, x + 120, y + 50, BLUE);
}

void UIManager::DrawInventorySection(const InventoryUIData& data, float x, float y) {
    DrawText("INVENTORY", x, y, 20, LIGHTGRAY);
    DrawSlotGrid(data.slots, data.capacity, data.columns, data.selectedIndex, data.draggedIndex, x, y + 40, false );
}

void UIManager::DrawContainerSection(const InventoryUIData& data, float x, float y) {
    if (!data.isLootingMode) return;

    DrawText("CONTAINER", x, y, 20, LIGHTGRAY);
    DrawSlotGrid(data.containerSlots, (int)data.containerSlots.size(), data.columns, -1, -1, x, y + 40, true );
}

void UIManager::DrawSlotGrid(
    const std::vector<UIRenderSlot>& slots,
    int capacity,
    int columns,
    int selectedIndex,
    int draggedIndex,
    float startX,
    float startY,
    bool isContainer
) {
    int maxSlotsToDraw = isContainer ? (columns * 2) : capacity;
    int drawCount = ((int)slots.size() < maxSlotsToDraw) ? (int)slots.size() : maxSlotsToDraw;

    for (int i = 0; i < drawCount; i++) {
        float slotX = startX + (i % columns) * SLOT_SIZE;
        float slotY = startY + (i / columns) * SLOT_SIZE;

        Rectangle slotRec = {slotX, slotY, SLOT_SIZE, SLOT_SIZE};

        Color slotColor = BLACK;

        if (isContainer && slots[i].isLocked) {
            if (std::sin(GetTime() * 20.0f) > 0.0f)
                slotColor = RED;
        }

        DrawRectangleRec(slotRec, slotColor);
        DrawRectangleLinesEx(slotRec, 1, LIGHTGRAY);

        if (!isContainer && slots[i].isLocked) {
           // DrawRectangleRec(slotRec, DARKGRAY);

            DrawLineEx(
                { slotX + 8, slotY + 8 },
                { slotX + SLOT_SIZE - 8, slotY + SLOT_SIZE - 8 },3,DARKGRAY
            );

            DrawLineEx(
                { slotX + SLOT_SIZE - 8, slotY + 8 },
                { slotX + 8, slotY + SLOT_SIZE - 8 },3, DARKGRAY
            );

            continue;
        }

        if (slots[i].itemPtr != nullptr && slots[i].count > 0) {
            Item* item = static_cast<Item*>(slots[i].itemPtr);

            item->SetPosition(slotX + 5, slotY + 5);
            item->SetActivated(false);
            item->Draw();

            if (slots[i].count > 1) {
                DrawText(TextFormat("%d", slots[i].count),slotX + 35,slotY + 35, 12, WHITE);
            }
        }

        if (!isContainer && i == selectedIndex) {
            DrawRectangleLinesEx(slotRec, 3, RED);
        }
    }
}

void UIManager::DrawInventoryUI(const InventoryUIData& data, const Player& player) {
    if (!data.isOpen) return;

    constexpr float NOTEBOOK_WIDTH = 720.0f;
    constexpr float NOTEBOOK_HEIGHT = 500.0f;

    float panelX = (GetScreenWidth() - NOTEBOOK_WIDTH) / 2.0f;
    float panelY = (GetScreenHeight() - NOTEBOOK_HEIGHT) / 2.0f;

    DrawNotebookBackground(panelX, panelY, NOTEBOOK_WIDTH, NOTEBOOK_HEIGHT);

    // =====================================================
    // LEFT PAGE
    // =====================================================

    float leftPageX = panelX + 25.0f;

    DrawProfileSection(player, leftPageX, panelY + 25.0f);

    // -----------------------------------------------------
    // OBJECTIVE
    // -----------------------------------------------------

    float objectiveY = panelY + 160.0f;

    DrawRectangle(leftPageX, objectiveY, 300, 180, Fade(BLACK, 0.35f));
    DrawRectangleLinesEx({ leftPageX, objectiveY, 300, 180 }, 2, LIGHTGRAY);

    DrawText("OBJECTIVE", leftPageX + 10, objectiveY + 10, 12, WHITE);
    DrawLine(leftPageX + 10, objectiveY + 35, leftPageX + 290, objectiveY + 35, LIGHTGRAY);

    float currentTextY = objectiveY + 50.0f;

    for (size_t i = 0; i < data.activeObjectives.size(); i++) {
        std::string titleText = "- " + data.activeObjectives[i].title; 
        DrawText(titleText.c_str(), leftPageX + 15, currentTextY, 13, WHITE);
        
        currentTextY += 18.0f;

        std::string descText = "  " + data.activeObjectives[i].description; 
        DrawText(descText.c_str(), leftPageX + 15, currentTextY, 10, LIGHTGRAY);

        currentTextY += 25.0f;
    }

    // -----------------------------------------------------
    // KEY BINDINGS
    // -----------------------------------------------------

    float keybindY = objectiveY + 190.0f;

    DrawRectangle(leftPageX, keybindY, 300, 130, Fade(BLACK, 0.35f));
    DrawRectangleLinesEx({ leftPageX, keybindY, 300, 130 }, 2, LIGHTGRAY);

    DrawText("KEY BINDINGS", leftPageX + 10, keybindY + 10, 12, WHITE);
    DrawLine(leftPageX + 10, keybindY + 35, leftPageX + 290, keybindY + 35, LIGHTGRAY);

    DrawText("[TAB] Open Inventory", leftPageX + 15, keybindY + 50, 11, LIGHTGRAY);
    DrawText("[E] Loot / Interact", leftPageX + 15, keybindY + 75, 11, LIGHTGRAY);
    DrawText("[Right Click] Item Menu", leftPageX + 15, keybindY + 100, 11, LIGHTGRAY);

    // =====================================================
    // RIGHT PAGE
    // =====================================================

    float rightPageX = panelX + (NOTEBOOK_WIDTH / 2.0f) + 40.0f;

    DrawInventorySection(data, rightPageX, panelY + 20.0f);
    DrawContainerSection(data, rightPageX, panelY + 300.0f);
}