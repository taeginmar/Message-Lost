#pragma once
#include "levels/level.h"

#include "gameplay/item/container.h"
#include "gameplay/item/item.h"
#include "gameplay/entity/enemy.h"
#include "gameplay/item/consumable.h"

Zone* CreateZoneA(Player& player);

Zone* CreateZoneB(Player& player);

void SetUpMockUpLevel(Level& level, Player& player);