#include "inventory.h"
#include "core/ui_manager.h"
#include <algorithm>
#include <cctype>

const float SLOT_SIZE = 50.0f;

// ==========================================
// CONSTRUCTOR / DESTRUCTOR
// ==========================================

Inventory::Inventory(int size, int cols)
    : capacity(size), columns(cols), isOpen(false), selectedIndex(0), draggedIndex(-1),
      pickTimer(0.0f), isLootingMode(false), linkedContainer(nullptr) 
{
    slots.resize(capacity);
    for (auto& slot : slots) {
        slot.isLocked = true;
    }

    contextMenu.show = false;
}

Inventory::~Inventory() {}

// ==========================================
// CORE LOGIC
// ==========================================
bool Inventory::AddItem(Item* item){
    if (item == nullptr)
        return false;

    std::string newItemType = item->GetItemType();
    
    std::string eventId = newItemType;
    std::transform(eventId.begin(), eventId.end(), eventId.begin(), [](unsigned char c){ return std::tolower(c); });

    for (int i = 0; i < capacity; i++) {
        if (slots[i].itemDetails != nullptr && !slots[i].isLocked) {
            if (slots[i].itemDetails->GetItemType() == newItemType) {
                if (slots[i].count >= MAX_STACK) {
                    TraceLog(LOG_WARNING, "STACK LIMIT");
                    return false;
                }

                slots[i].count += item->GetStackCount();
                gObjectiveManager.ReportEvent(ObjectiveType::COLLECT_ITEM, eventId, item->GetStackCount());
                return true;
            }
        }
    }

    for (int i = 0; i < capacity; i++) {
        if (slots[i].itemDetails == nullptr && !slots[i].isLocked){
            slots[i].itemDetails = item;
            slots[i].count = item->GetStackCount();
            
            gObjectiveManager.ReportEvent(ObjectiveType::COLLECT_ITEM, eventId, item->GetStackCount());
            TraceLog(LOG_INFO, "ADD ITEM TO SLOT %d", i);

            return true;
        }
    }

    TraceLog(LOG_WARNING, "NO FREE SLOT");

    return false;
}

void Inventory::RemoveItem(int index)
{
    if(index < 0 || index >= capacity)
        return;

    if(slots[index].itemDetails == nullptr)
        return;

    slots[index].count--;

    slots[index].itemDetails->SetStackCount(slots[index].count);

    if(slots[index].count <= 0)
    {
        slots[index].itemDetails = nullptr;
        slots[index].count = 0;
    }
}

void Inventory::MoveItem(int fromIndex, int toIndex) {
    if (fromIndex >= 0 && fromIndex < capacity && toIndex >= 0 && toIndex < capacity) {
        InventorySlot temp = slots[fromIndex];
        slots[fromIndex] = slots[toIndex];
        slots[toIndex] = temp;
    }
}

void Inventory::OpenContainer(Container* container) {
    if (container == nullptr) return;
    isOpen = true;
    isLootingMode = true;
    linkedContainer = container;
    contextMenu.show = false;
}

void Inventory::Toggle() {
    isOpen = !isOpen;
    contextMenu.show = false;
    draggedIndex = -1;
    if (!isOpen) {
        isLootingMode = false;
        linkedContainer = nullptr;
    }
}

bool Inventory::IsOpen() const { return isOpen; }
void Inventory::UnlockSlot(int index) { if (index >= 0 && index < capacity) slots[index].isLocked = false; }
void Inventory::UnlockSlots(int count) { for (int i = 0; i < count && i < capacity; i++) slots[i].isLocked = false; }

// ==========================================
// HANDLERS
// ==========================================

void Inventory::HandleItemInteraction(Player& player, std::vector<Item*>& worldItems)
{
    if (isOpen)return;

    for (auto it = worldItems.begin(); it != worldItems.end(); ++it){
        Item* item = *it;

        if (item == nullptr) continue;

        if (!item->IsActivated() && CheckCollisionRecs(player.GetBounds(), item->GetBounds())){
            if (IsKeyPressed(KEY_E)){
                bool added = AddItem(item);
                if (added) worldItems.erase(it);

                break;
            }
        }
    }
}

void Inventory::HandleSlotHover(Vector2 mousePos, float startX, float startY) {
    selectedIndex = -1;
    for (int i = 0; i < capacity; i++) {
        if (CheckCollisionPointRec(mousePos, { startX + (i % columns) * SLOT_SIZE, startY + (i / columns) * SLOT_SIZE, SLOT_SIZE, SLOT_SIZE })) {
            if (!slots[i].isLocked) {
                selectedIndex = i;
            }
            break;
        }
    }
}

void Inventory::HandleDragAndDrop(Vector2 mousePos, float startX, float startY) {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (selectedIndex >= 0) {
            if (draggedIndex == -1) { if (slots[selectedIndex].itemDetails) draggedIndex = selectedIndex; }
            else { MoveItem(draggedIndex, selectedIndex); draggedIndex = -1; }
        } else draggedIndex = -1;
    }
}

void Inventory::HandleContextMenuTrigger(Vector2 mousePos, float startX, float startY) {
    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && selectedIndex != -1 && slots[selectedIndex].itemDetails != nullptr) {
        contextMenu.show = true; contextMenu.index = selectedIndex; contextMenu.pos = mousePos;
    }
}

void Inventory::ExecuteAction(int action, int index, Player& player, std::vector<Item*>& worldItems) {
    if (index < 0 || index >= capacity) return;
    if (slots[index].itemDetails == nullptr) return;

    Item* item = slots[index].itemDetails;

    switch (action) {
        case 0:{
            bool success = item->ApplyEffect(player);
            if (success) {
                slots[index].count--;

                if (slots[index].count <= 0) {
                    slots[index].itemDetails = nullptr;
                    slots[index].count = 0;
                }
            }
            break;
        }
        case 1:{
            break;
        }
        case 2:{
            item->SetStackCount(slots[index].count);
            item->SetPosition(player.GetBounds().x, player.GetBounds().y);

            item->SetActivated(false);
            worldItems.push_back(item);

            slots[index].itemDetails = nullptr;
            slots[index].count = 0;

            break;
        }
    }
    contextMenu.show = false;
}

void Inventory::Update(Player& player, std::vector<Item*>& worldItems) {
    if (pickTimer > 0.0f){
        pickTimer -= GetFrameTime();
    }

    if(stackLimitWarningTimer > 0.0f){
        stackLimitWarningTimer -= GetFrameTime();
    }

    HandleItemInteraction(player, worldItems);
    if (!isOpen) return;

    float rightPageX = (GetScreenWidth() - 720.0f) / 2.0f + 400.0f;
    float invStartY = (GetScreenHeight() - 500.0f) / 2.0f + 60.0f;
    Vector2 mousePos = GetMousePosition();

    HandleSlotHover(mousePos, rightPageX, invStartY);

    if (isLootingMode && linkedContainer != nullptr && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        float lootStartY = invStartY + 280.0f;
        auto& containerItems = linkedContainer->GetItems();
        
        for (int i = 0; i < (int)containerItems.size(); i++) {
            float slotX = rightPageX + (i % columns) * SLOT_SIZE;
            float slotY = lootStartY + (i / columns) * SLOT_SIZE;
            
            if (CheckCollisionPointRec(mousePos, { slotX, slotY, SLOT_SIZE, SLOT_SIZE })) {
                if (containerItems[i] != nullptr) {
                    if (AddItem(containerItems[i])) {
                        linkedContainer->RemoveItem(i); 
                        break; 
                    }
                    else{
                        stackLimitWarningSlot = i;
                        stackLimitWarningTimer = 0.8f;
                    }
                }
            }
        }
    }

    if (contextMenu.show) {
        contextMenu.Update(mousePos);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (CheckCollisionPointRec(mousePos, { contextMenu.pos.x, contextMenu.pos.y, 100, 90 })) {
                ExecuteAction(contextMenu.selectedOpt, contextMenu.index, player, worldItems);
            } else contextMenu.show = false;
        }
    } else {
        HandleDragAndDrop(mousePos, rightPageX, invStartY);
        HandleContextMenuTrigger(mousePos, rightPageX, invStartY);
    }
}

void Inventory::Draw(const Player& player) {
    InventoryUIData data;
    data.capacity = capacity;
    data.columns = columns;
    data.selectedIndex = selectedIndex;
    data.draggedIndex = draggedIndex;
    data.isOpen = isOpen;
    data.isLootingMode = isLootingMode;
    data.hp = player.GetHealth();
    data.maxHp = 100.0f; 
    data.stamina = player.GetStamina();

    data.activeObjectives.clear();
    
    Objective* currentObj = gObjectiveManager.GetCurrentObjective();
    if (currentObj != nullptr && !currentObj->IsCompleted()) {
        data.activeObjectives.push_back({
            currentObj->GetTitle(),
            currentObj->GetDescription()
        });
    }

    const auto& objectives = gObjectiveManager.GetObjectives();
    for (const auto& obj : objectives) {
        if (obj != nullptr && !obj->IsCompleted() && obj->IsUnlocked()) {
            if (currentObj == nullptr || obj->GetTitle() != currentObj->GetTitle()) {
                data.activeObjectives.push_back({
                    obj->GetTitle(),
                    obj->GetDescription()
                });
            }
        }
    }

    data.slots.clear(); 
    for(int i = 0; i < capacity; i++) {
        data.slots.push_back({ slots[i].isLocked, (void*)slots[i].itemDetails, slots[i].count, (i == draggedIndex) });
    }

    data.containerSlots.clear(); 
    if (isLootingMode && linkedContainer != nullptr) {
        auto& items = linkedContainer->GetItems();
        for (int i = 0; i < (int)items.size(); i++) {
            bool flash = false;

            if (i == stackLimitWarningSlot && stackLimitWarningTimer > 0.0f){
                flash = true;
            }

            data.containerSlots.push_back({flash, (void*)items[i],1,false});
        }
        while(data.containerSlots.size() < (size_t)capacity) {
            data.containerSlots.push_back({ false, nullptr, 0, false });
        }
    }
    
    UIManager::DrawInventoryUI(data, player);
    
    if (contextMenu.show) contextMenu.Draw();
}

void Inventory::Clear(){
    for(auto& slot : slots){
        slot.itemDetails = nullptr;
        slot.count = 0;
    }

    selectedIndex = 0;
    draggedIndex = -1;

    isOpen = false;
    isLootingMode = false;

    linkedContainer = nullptr;

    stackLimitWarningTimer = 0.0f;
    stackLimitWarningSlot = -1;
}

int Inventory::GetItemCount(const std::string& targetItemType) const {
    int totalCount = 0;
    for (int i = 0; i < capacity; i++) {
        if (!slots[i].isLocked && slots[i].itemDetails != nullptr) {
            Item* item = static_cast<Item*>(slots[i].itemDetails);
            if (item->GetItemType() == targetItemType) {
                totalCount += slots[i].count;
            }
        }
    }
    return totalCount;
}

void InventoryContextMenu::Update(Vector2 mousePos) { for (int i = 0; i < 3; i++) if (CheckCollisionPointRec(mousePos, { pos.x, pos.y + (i * 28), 100, 28 })) selectedOpt = i; }
void InventoryContextMenu::Draw() {
    DrawRectangle(pos.x, pos.y, 100, 90, Fade(BLACK, 0.9f));
    for (int i = 0; i < 3; i++) DrawText(option[i].c_str(), pos.x + 10, pos.y + 7 + (i * 28), 16, (i == selectedOpt) ? RED : WHITE);
}