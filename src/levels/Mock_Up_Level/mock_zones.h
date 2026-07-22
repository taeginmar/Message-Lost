#pragma once
#include "levels/level.h"

#include "gameplay/item/container.h"
#include "gameplay/item/item.h"
#include "gameplay/entity/enemy.h"
#include "gameplay/entity/player.h"
#include "gameplay/item/consumable.h"
#include "gameplay/item/equipment.h"

#include "core/objective.h"
#include "core/objective_manager.h"
#include "core/objective_box.h"

// Create Level Structure
Zone* CreateZoneA(Player& player);
Zone* CreateZoneB(Player& player, Zone* zoneA); // [ปรับแก้] รับ zoneA เพิ่มเติมเพื่อส่งต่อให้ MeetHelmuth
void SetUpMockUpLevel(Level& level, Player& player);
// Mission

// Objective 1
class GeneratorBox : public Interactable {
private:
    bool isActivated;
public:
    GeneratorBox(float x, float y);
    void OnInteract() override;
    void Draw() override;
};

// Objective 2
class MeetHelmuth : public Interactable {
private:
    bool isTalked;
    Player& targetPlayer; // [ปรับเพิ่ม] เก็บ reference ของ Player เพื่อใช้สร้าง Enemy
    Zone* targetZoneA;   // [ปรับเพิ่ม] เก็บ reference ของ ZoneA เพื่อใช้สปอน Enemy
public:
    MeetHelmuth(float x, float y, Player& player, Zone* zoneA); // [ปรับแก้] ปรับพารามิเตอร์รับ player และ zoneA
    void OnInteract() override;
    void Draw() override;
};

// [ตัดโค้ด] ลบคลาส MedKitCollect ออกทั้งหมด เพื่อไปใช้คลาส Medkit จาก consumable.h โดยตรง

// Objective 4
class EnemyNeutralize : public Enemy {
private:
    bool eventReported;
public:
    EnemyNeutralize(float x, float y, Player* player);
    void Draw() override;
    void OnDeath() override;
};

// Objective 5
class FindExit : public Interactable {
public:
    FindExit(float x, float y);
    void OnInteract() override;
    void Draw() override;
};