#pragma once
#include "levels/level.h"

#include "gameplay/item/container.h"
#include "gameplay/item/item.h"
#include "gameplay/entity/enemy.h"
#include "gameplay/item/consumable.h"
#include "gameplay/item/equipment.h"

#include "core/objective.h"
#include "core/objective_manager.h"
#include "core/objective_box.h"

Zone* CreateZoneA(Player& player);
Zone* CreateZoneB(Player& player);
void SetUpMockUpLevel(Level& level, Player& player);

class GeneratorBox : public Interactable {
public:
    GeneratorBox(float x, float y) : Interactable(x, y, 40.0f, 40.0f, "Press E to Activate Generator") {}

    void OnInteract() override;
    void Draw() override;
};