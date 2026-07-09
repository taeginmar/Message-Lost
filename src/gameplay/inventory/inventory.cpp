#include "inventory.h"

//private
void Inventory::DrawItemIcon(Item* item, float x, float y){
    Rectangle miniBounds = {x, y, 40, 40};

    if(item->GetPrompt().find("MedKit") != std::string::npos){
        DrawRectangleRec(miniBounds, RED);
        DrawRectangle(x + 17, y + 8, 6, 24, WHITE); // แกนตั้งเครื่องหมายบวก (แก้พิกัด x เป็น y ให้ถูกต้อง)
        DrawRectangle(x + 8, y + 17, 24, 6, WHITE); // แกนนอนเครื่องหมายบวก (แก้พิกัด x เป็น y ให้ถูกต้อง)
    }else{
        DrawRectangleRec(miniBounds, ORANGE);
    }
}

void Inventory::ExecuteContextAction(int action, int index){
    if(slots[index] == nullptr) return;

    if(action == 0){
        slots[index]->OnInteract();
        RemoveItem(index);
    }else if(action == 1){
        TraceLog(LOG_INFO, "INSPECT: %s", slots[index]->GetPrompt().c_str()); // แก้ไขฟอร์แมต String ล็อกล็อกอินโฟ
    }else if(action == 2){
        RemoveItem(index);
    }
}

//public
bool Inventory::AddItem (Item* item){
    if(item == nullptr) return false;
    for(int i = 0; i < capacity; i++){
        if(slots[i] == nullptr){
            slots[i] = item;
            return true;
        }
    }
    return false;
}

void Inventory::RemoveItem(int index){
    if(index >= 0 && index < capacity){
        slots[index] = nullptr;
    }
}

void Inventory::MoveItem(int fromIndex, int toIndex){
    if(fromIndex >= 0 && fromIndex < capacity && toIndex >= 0 && toIndex < capacity){
        Item* temp = slots[fromIndex];
        slots[fromIndex] = slots[toIndex];
        slots[toIndex] = temp;
    }
}

void Inventory::Toggle(){
    isOpen = !isOpen;
    showContextMenu = false;
    draggedIndex = -1;
}

void Inventory::Update(Player& player, std::vector<Item*>& items) {
    float dt = GetFrameTime();

    // 🌟 1. ลอจิกนับเวลาถอยหลังการแสดงรูปไอเทมขนาดใหญ่ที่เพิ่งเก็บ
    if (pickTimer > 0.0f) {
        pickTimer -= dt;
    }

    // 🌟 2. ดึงลอจิกการตรวจจับการเก็บไอเทม (Pick Item) จาก main.cpp มาไว้ที่นี่
    if (!isOpen) { // เก็บไอเทมได้เฉพาะตอนปิดหน้าต่างกระเป๋า
        Vector2 playerPos = { player.GetBounds().x, player.GetBounds().y };
        
        for (auto item : items) {
            if (!item->IsActivated() && CheckCollisionRecs(player.GetBounds(), item->GetBounds())) {
                if (IsKeyPressed(KEY_E)) {
                    // ตรวจสอบชนิดไอเทมก่อนเก็บเข้าคลัง
                    if (item->GetPrompt().find("MedKit") != std::string::npos) {
                        lastPickedItemType = "MedKit";
                    } else if (item->GetPrompt().find("Energy") != std::string::npos) {
                        lastPickedItemType = "EnergyDrink";
                    }

                    // สั่งเพิ่มเข้าช่องเก็บของ
                    if (AddItem(item)) {
                        item->OnInteract(); // ซ่อนไอเทมจากพื้นแผนที่
                        pickTimer = 1.5f;   // ตั้งเวลาแสดงรูปใหญ่ 1.5 วินาที
                    }
                }
            }
        }
        return; // ถ้าปิดกระเป๋าอยู่ ไม่ต้องประมวลผลลอจิกการคลิกในกระเป๋าด้านล่าง
    }

    // --- ด้านล่างนี้คือลอจิกควบคุมกระเป๋าด้วยเมาส์เดิมของคุณตามปกติ ---
    Vector2 mousePos = GetMousePosition();
    float startX = 400 - (columns * 60) / 2;
    float startY = 300 - ((capacity / columns) * 60) / 2;

    for(int i = 0; i < capacity; i++){
        float slotX = startX + (i % columns) * 60;
        float slotY = startY + (i / columns) * 60;
        if(CheckCollisionPointRec(mousePos, Rectangle{slotX, slotY, 50, 50})){
            selectedIndex = i;
            break;
        }
    }

    if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && !showContextMenu){
        float slotX = startX + (selectedIndex % columns) * 60;
        float slotY = startY + (selectedIndex / columns) * 60;
        if(CheckCollisionPointRec(mousePos, Rectangle{slotX, slotY, 50, 50}) && slots[selectedIndex] != nullptr){
            showContextMenu = true;
            contextIndex = selectedIndex;
            contextMenuPos = mousePos;
            contextMenuSelected = 0;
        }
    }

    if(showContextMenu){
        for(int i = 0; i < 3; i++){
            Rectangle optRec = { contextMenuPos.x, contextMenuPos.y + (i * 28), 100, 28 };
            if(CheckCollisionPointRec(mousePos, optRec)){
                contextMenuSelected = i;
                if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                    // 🌟 ส่ง player เข้าไปประมวลผลเอฟเฟกต์การใช้งาน
                    ExecuteContextAction(contextMenuSelected, contextIndex); 
                    showContextMenu = false;
                    return;
                }
            }
        }
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !CheckCollisionPointRec(mousePos, Rectangle{contextMenuPos.x, contextMenuPos.y, 100, 90})){
            showContextMenu = false;
        }
        return;
    }

    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        float slotX = startX + (selectedIndex % columns) * 60;
        float slotY = startY + (selectedIndex / columns) * 60;
        if(CheckCollisionPointRec(mousePos, Rectangle{slotX, slotY, 50, 50}) && slots[selectedIndex] != nullptr){
            draggedIndex = selectedIndex;
        }
    }

    if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && draggedIndex != -1){
        float slotX = startX + (selectedIndex % columns) * 60;
        float slotY = startY + (selectedIndex / columns) * 60;
        if(CheckCollisionPointRec(mousePos, Rectangle{slotX, slotY, 50, 50})){
            MoveItem(draggedIndex, selectedIndex);
        }
        draggedIndex = -1;
    }
}

void Inventory::Draw(const Player& player){
    // 🌟 1. ดึงลอจิกการวาดรูปไอเทมขนาดใหญ่ตอนเก็บ (Visual Feedback) มาไว้ที่นี่
    if (pickTimer > 0.0f && player.GetHealth() > 0) {
        DrawRectangle(320, 220, 160, 160, Fade(GRAY, 0.8f));
        DrawRectangleLines(320, 220, 160, 160, WHITE);
        
        if (lastPickedItemType == "MedKit") {
            DrawRectangle(360, 260, 80, 80, RED);
            DrawRectangle(392, 275, 16, 50, WHITE);
            DrawRectangle(375, 392, 50, 16, WHITE); 
            DrawText("MEDKIT ACQUIRED", 310, 400, 20, GREEN);
        } 
        else if (lastPickedItemType == "EnergyDrink") {
            DrawRectangle(370, 250, 60, 100, ORANGE);
            DrawRectangle(385, 250, 30, 100, BLUE); 
            DrawText("ENERGY DRINK ACQUIRED", 280, 400, 20, GOLD);
        }
    }

    if(!isOpen) return;

    // --- ส่วนการวาดหน้าต่าง Inventory เดิม (คงเดิมไว้ทั้งหมด) ---
    int rows = capacity / columns;
    float menuWidth = columns * 60 + 40;
    float menuHeight = rows * 60 + 60;
    float startX = 400 - (menuWidth / 2);
    float startY = 300 - (menuHeight / 2);

    DrawRectangle(startX, startY, menuWidth, menuHeight, Fade(DARKGRAY, 0.9f));
    DrawRectangleLines(startX, startY, menuWidth, menuHeight, WHITE);
    DrawText("INVENTORY", startX + 20, startY + 15, 20, LIGHTGRAY);

    float gridStartX = startX + 20;
    float gridStartY = startY + 50;

    for(int i = 0; i < capacity; i++){
        float slotX = gridStartX + (i % columns) * 60;
        float slotY = gridStartY + (i / columns) * 60;
        Color boxBorderColor = (i == selectedIndex)? YELLOW : GRAY;
        int borderThickness = (i == selectedIndex)? 3 : 1;
        DrawRectangle(slotX, slotY, 50, 50, BLACK);
        DrawRectangleLinesEx(Rectangle{slotX, slotY, 50, 50}, borderThickness, boxBorderColor);
        if(slots[i] != nullptr && i != draggedIndex){
            DrawItemIcon(slots[i], slotX + 5, slotY + 5);
        }
    }

    if(draggedIndex != -1 && slots[draggedIndex] != nullptr){
        Vector2 mousePos = GetMousePosition();
        DrawItemIcon(slots[draggedIndex], mousePos.x - 20, mousePos.y - 20);
    }

    if(showContextMenu){
        DrawRectangle(contextMenuPos.x, contextMenuPos.y, 100, 90, RAYWHITE);
        DrawRectangleLines(contextMenuPos.x, contextMenuPos.y, 100, 90, BLACK);
        std::string menuOpts[3] = {"Use", "Inspect", "Remove"};
        for(int i = 0; i < 3; i++){
            Color txtColor = (i == contextMenuSelected)? RED : BLACK;
            if(i == contextMenuSelected){
                DrawRectangle(contextMenuPos.x + 2, contextMenuPos.y + 2 + (i * 28), 96, 26, LIGHTGRAY);
            }
            DrawText(menuOpts[i].c_str(), contextMenuPos.x + 10, contextMenuPos.y + 7 + (i * 28), 16, txtColor);
        }
    }
}