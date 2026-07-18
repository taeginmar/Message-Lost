#include "levels/Mock_Up_Level/mock_zones.h"

Zone* CreateZoneA(Player& player) {
    Zone* zone = new Zone("Warehouse A");
    
    zone->walls.push_back({0, 0, 800, 20});
    zone->walls.push_back({0, 0, 20, 600});

    Container* containerA = new Container({100, 100, 50, 50});
    
    containerA->AddItem(new Medkit(0.0f, 0.0f, player));
    containerA->AddItem(new EnergyDrink(0.0f, 0.0f, player));
    containerA->AddItem(new EnergyDrink(0.0f, 0.0f, player));

    zone->AddContainer(containerA);
    
    zone->AddItems(new Medkit(200.0f, 200.0f, player));
    zone->AddItems(new EnergyDrink(300.f, 200.0f, player));

    return zone;
}

Zone* CreateZoneB(Player& player) {
    Zone* zone = new Zone("Security Room B");
    
    zone->walls.push_back({300, 200, 400, 20});
    
    zone->AddEnemy(new Enemy(400, 300, 40, 40, 100, 100, 200, &player, 
                             Enemy::NormalAction::IDLE, Enemy::PatrolType::HORIZONTAL, 
                             1, 150.0f, 10.0f));
    
    Container* containerB = new Container({500, 400, 50, 50});

    containerB->AddItem(new Medkit(0.0f, 0.0f, player));
    containerB->AddItem(new Medkit(0.0f, 0.0f, player));
    containerB->AddItem(new Medkit(0.0f, 0.0f, player));

    zone->AddContainer(containerB);
    
    return zone;
}

void SetUpMockUpLevel(Level& level, Player& player) {
    Zone* zoneA = CreateZoneA(player);
    Zone* zoneB = CreateZoneB(player);

    level.AddZone(zoneA);
    level.AddZone(zoneB);

    level.AddDoor(Door({750, 250, 50, 100}, 1, {100, 300}, 0)); 
    level.AddDoor(Door({20, 250, 50, 100}, 0, {400, 300}, 1));
}