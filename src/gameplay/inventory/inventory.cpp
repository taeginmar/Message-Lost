#include "inventory.h"

//Context Menu 
void InventoryContextMenu::Update(Vector2 mousePos){
    if(!show) return;
    for(int i = 0; i < 3; i++){
        Rectangle optRec = {pos.x, pos.y + (i * 28), 100, 28};
        if(CheckCollisionPointRec(mousePos, optRec)){
            selectedOpt = i;
            break;
        }
    }
}

void InventoryContextMenu::Draw(){
    if(!show) return;
    DrawRectangle(pos.x, pos.y, 100, 90, RAYWHITE);
    DrawRectangle(pos.x, pos.y, 100, 90, BLACK);

    for(int i = 0; i < 3; i++){
        Color txtColor = (i == selectedOpt)? RED : BLACK;
        if(i == selectedOpt){
            DrawRectangle(pos.x + 2, pos.y + 2 + (i * 28), 96, 26, LIGHTGRAY);
        }
        DrawText(option[i].c_str(), pos.x + 10, pos.y + 7 + (i * 28), 16, txtColor);
    }
}

//Inventory Core Usage
Inventory::Inventory(int size, int cols)
    :capacity(size), columns(cols), isOpen(false), selectedIndex(0), draggedIndex(-1),
    lastPickedItemType(""), pickTimer(0.0f)
{
    slots.resize(capacity);
}


Inventory::~Inventory() {}

bool Inventory::AddItem(Item* item){
if (item == nullptr) return false;

    bool oldState = item->IsActivated();
    item->SetActivated(false);
    std::string newItemType = item->GetItemType();
    item->SetActivated(oldState);

    for (int i = 0; i < capacity; i++) {
        if (slots[i].itemDetails != nullptr) {
            bool oldSlotState = slots[i].itemDetails->IsActivated();
            slots[i].itemDetails->SetActivated(false);
            std::string currentItemType = slots[i].itemDetails->GetItemType();
            slots[i].itemDetails->SetActivated(oldSlotState);
            
            if ((newItemType.find("MedKit") != std::string::npos && currentItemType.find("MedKit") != std::string::npos) ||
                (newItemType.find("Energy") != std::string::npos && currentItemType.find("Energy") != std::string::npos)) {
                
                slots[i].count++; // เพิ่มจำนวนชิ้นในสล็อตเดิม
                return true;      // สั่งออกฟังก์ชันทันทีเพื่อไม่ให้หลุดไปเปิดช่องใหม่
            }
        }
    }

    for(int i = 0; i < capacity; i++){
        if(slots[i].itemDetails == nullptr){
            slots[i].itemDetails = item;
            slots[i].count = 1;
            return true;
        }
    }
    return false;
}

void Inventory::RemoveItem(int index){
    if(index >= 0 && index < capacity){
        slots[index].count--;
        if(slots[index].count <= 0){
            slots[index].itemDetails = nullptr;
            slots[index].count = 0;
        }
    }
}

void Inventory::MoveItem(int fromIndex, int toIndex){
    if(fromIndex >= 0 && fromIndex < capacity && toIndex >= 0 && toIndex < capacity){
        InventorySlot temp = slots[fromIndex];
        slots[fromIndex] = slots[toIndex];
        slots[toIndex] = temp;
    }
}

void Inventory::Toggle() {
    isOpen = !isOpen;
    contextMenu.show = false;
    draggedIndex = -1;
}

bool Inventory::IsOpen() const { return isOpen; }

//Logic Segment
void Inventory::HandleItemInteraction(Player& player, std::vector<Item*>& worldItems){
    if(isOpen) return;

    for(auto item : worldItems){
        if(!item->IsActivated() && CheckCollisionRecs(player.GetBounds(), item->GetBounds())){
            if(IsKeyPressed(KEY_E)){
                if(item->GetPrompt().find("MedKit") != std::string::npos) lastPickedItemType = "MedKit";
                else if(item->GetPrompt().find("Energy") != std::string::npos) lastPickedItemType = "Energy Drink";

                if(AddItem(item)){
                    item->OnInteract();
                    pickTimer = 1.5f;
                }
            }
        }
    }
}

void Inventory::HandleSlotHover(Vector2 mousePos, float startX, float startY){
    if(contextMenu.show) return;
    
    for(int i = 0; i < capacity; i++){
        float slotX = startX + (i % columns) * 60;
        float slotY = startY + (i / columns) * 60;
        if(CheckCollisionPointRec(mousePos, Rectangle{slotX, slotY, 50, 50})){
            selectedIndex = i;
            break;
        }
    }
}

void Inventory::HandleContextMenuTrigger(Vector2 mousePos, float startX, float startY){
    if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)){
        
        if (contextMenu.show) {
            contextMenu.show = false;
        } 
        else {
            float slotX = startX + (selectedIndex % columns) * 60;
            float slotY = startY + (selectedIndex / columns) * 60;

            if(CheckCollisionPointRec(mousePos, Rectangle{slotX, slotY, 50, 50}) && slots[selectedIndex].itemDetails != nullptr){
                contextMenu.show = true;
                contextMenu.index = selectedIndex;
                contextMenu.pos = mousePos;
                contextMenu.selectedOpt = 0;
            }
        }
    }
}

void Inventory::HandleDragAndDrop(Vector2 mousePos, float startX, float startY) {
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        float slotX = startX + ( selectedIndex % columns ) * 60;
        float slotY = startY + ( selectedIndex / columns ) * 60;
        if(CheckCollisionPointRec(mousePos, Rectangle{slotX, slotY, 50, 50})){
            MoveItem(draggedIndex, selectedIndex);
        }
        draggedIndex = -1;
    }
}

void Inventory::ExecuteAction(int action, int index, Player& player, std::vector<Item*>& worldItems) {
    if (slots[index].itemDetails == nullptr) return;

    Item* currentItem = slots[index].itemDetails;

    if (action == 0) { // คำสั่ง USE
        bool oldState = currentItem->IsActivated();
        currentItem->SetActivated(false);
        std::string itemName = currentItem->GetItemType();
        currentItem->SetActivated(oldState);

        float currentHP = player.GetHealth();
        float currentStamina = player.GetStamina();

    if (itemName.find("MedKit") != std::string::npos) {
        player.Heal(40.0f); // เลือดเพิ่มทันที และไม่เกิน MaxHealth
    } else if (itemName.find("Energy") != std::string::npos) {
        player.RestoreStamina(50.0f); // สเตมิน่าเพิ่มทันที
    }

        RemoveItem(index); // หักจำนวนไอเทมออก
    } 
    else if (action == 1) { // คำสั่ง INSPECT
        TraceLog(LOG_INFO, "INSPECT: %s", currentItem->GetItemType().c_str());
    } 
    else if (action == 2) { // คำสั่ง REMOVE
        currentItem->SetPosition(player.GetBounds().x + 5.0f, player.GetBounds().y + 5.0f);
        currentItem->SetActivated(false); 
        RemoveItem(index);
    }
}

//Specifically Relate to Visualization
void Inventory::Update(Player& player, std::vector<Item*>& worldItems){
    if(pickTimer > 0.0f) pickTimer -= GetFrameTime();

    HandleItemInteraction(player, worldItems);

    if(!isOpen) return;

    Vector2 mousePos = GetMousePosition();
    float startX = 200;
    float startY = 100;

    if(isLootingMode && linkedContainer != nullptr){
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            float containerY = startY + 250;
            for(int i = 0; i < linkedContainer->GetItems().size(); i++){
                float slotX = startX + (i % columns) * 60;
                float slotY = containerY + (i / columns) * 60;

                if(CheckCollisionPointRec(mousePos, Rectangle{slotX, slotY, 50, 50})){
                    Item* itemToLoot = linkedContainer->GetItems()[i];
                    if(AddItem(itemToLoot)){
                        linkedContainer->RemoveItem(i);
                    }
                    break;
                }
            }
        }
    }

    HandleSlotHover(mousePos, startX, startY);
    HandleContextMenuTrigger(mousePos, startX, startY);

    if(contextMenu.show){
        contextMenu.Update(mousePos);
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            Rectangle menuRec = {contextMenu.pos.x, contextMenu.pos.y, 100, 90};
            if(CheckCollisionPointRec(mousePos, menuRec)){
                ExecuteAction(contextMenu.selectedOpt, contextMenu.index, player, worldItems);
            }
            contextMenu.show = false;
        }
        return;
    }

    HandleDragAndDrop(mousePos, startX, startY);
}

void Inventory::DrawDualGrid(float startX, float startY){
    DrawSlotGrid(startX, startY);
    
    float containerY = startY + 250;
    DrawText("Container", startX, containerY - 20, 20, YELLOW);

    const auto& containerItems = linkedContainer->GetItems();
    for(int i = 0; i < 8; i++){
        float slotX = startX + (i % columns) * 60;
        float slotY = containerY + (i / columns) * 60;

        DrawRectangle(slotX, slotY, 50, 50, BLACK);
        DrawRectangleLines(slotX, slotY, 50, 50, GRAY);

        if(i < containerItems.size()){
            Item* target = containerItems[i];
            target->SetPosition(slotX + 10, slotY + 10);
            target->Draw();
        }
    }
}

void Inventory::DrawSlotGrid(float startX, float startY){
    for(int i = 0; i < capacity; i++){
        float slotX = startX + (i % columns) * 60;
        float slotY = startY + (i / columns) * 60;

        Color boxBorderColor = (i == selectedIndex)? YELLOW : GRAY;
        int borderThickness = (i == selectedIndex)? 3 : 1;

        DrawRectangle(slotX, slotY, 50, 50, BLACK);
        DrawRectangleLinesEx(Rectangle{slotX, slotY, 50, 50}, borderThickness, boxBorderColor);

        if(slots[i].itemDetails != nullptr && i != draggedIndex){
            Item* target = slots[i].itemDetails;
            Rectangle oldBounds = target->GetBounds();
            bool oldState = target->IsActivated();

            target->SetPosition(slotX + 10, slotY + 10);
            target->SetActivated(false);
            target->Draw();

            target->SetActivated(oldState);
            target->SetBounds(oldBounds);

            if(slots[i].count > 0){
                DrawText(TextFormat("%d", slots[i].count), slotX + 38, slotY + 36, 12, WHITE);
            }
        }
    }
}

void Inventory::Draw(const Player& player){
    if(pickTimer > 0.0f && player.GetHealth() > 0){
        DrawRectangle(320, 220, 160, 160, Fade(GRAY, 0.8f));
        DrawRectangleLines(320, 220, 160, 160, WHITE);
        if(lastPickedItemType == "Medkit"){
            DrawRectangle(360, 260, 80, 80, RED);
            DrawRectangle(392, 266, 16, 68, WHITE); DrawRectangle(366, 392, 68, 16, WHITE);
            DrawText("MEDKIT ACQUIRED", 310, 400, 20, GREEN);
        }else if(lastPickedItemType == "Energy Drink"){
            DrawRectangle(370, 250, 60, 100, ORANGE); DrawRectangle(385, 250, 30, 100, BLUE);
            DrawText("ENERGY DRINK ACQUIRED", 280, 400, 20, GOLD); 
        }
    }

    if(!isOpen) return;

    float startX = 200;
    float startY = 100;

    DrawRectangle(startX - 20, startY - 20, 300, 500, Fade(DARKGRAY, 0.9f));
    DrawText("PLAYER INVENTORY", startX, startY - 10, 20, LIGHTGRAY);

    if (isLootingMode) {
        DrawDualGrid(startX, startY);
    } else {
        DrawSlotGrid(startX, startY);
    }

    if(draggedIndex != -1 && slots[draggedIndex].itemDetails != nullptr){
         Vector2 mousePos = GetMousePosition();
         Item* target = slots[draggedIndex].itemDetails;
         Rectangle oldBounds = target->GetBounds();
         bool oldState = target->IsActivated();

         target->SetPosition(mousePos.x - 10, mousePos.y - 10);
         target->SetActivated(false);
         target->Draw();

         target->SetActivated(oldState);
         target->SetBounds(oldBounds);

         DrawText(TextFormat("%d", slots[draggedIndex].count), mousePos.x + 15, mousePos.y + 15, 12, WHITE);
    }

    contextMenu.Draw();
}