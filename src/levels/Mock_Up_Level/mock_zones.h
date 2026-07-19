#pragma once
#include "levels/level.h"

#include "gameplay/item/container.h"
#include "gameplay/item/item.h"
#include "gameplay/entity/enemy.h"
#include "gameplay/item/consumable.h"
#include "gameplay/item/equipment.h"

Zone* CreateZoneA(Player& player);

Zone* CreateZoneB(Player& player);

void SetUpMockUpLevel(Level& level, Player& player);