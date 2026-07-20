#include "levels/Mock_Up_Level/mock_zones.h"

void GeneratorBox::OnInteract(){
    if (!isActivated) {
        isActivated = true;
        gObjectiveManager.ReportEvent(ObjectiveType::INTERACT, "generator_box");
    }
}

void GeneratorBox::Draw(){
        Color boxColor = isActivated ? GREEN : RED;
        DrawRectangleRec(bounds, boxColor);
        DrawText("GEN", bounds.x + 5, bounds.y + 15, 12, WHITE);
    }

Zone* CreateZoneA(Player& player) {
    Zone* zone = new Zone("Warehouse A");
    
    zone->walls.push_back({0, 0, 800, 20});
    zone->walls.push_back({0, 0, 20, 600});

    Container* containerA = new Container({100, 100, 50, 50});
    
    containerA->AddItem(new Medkit(0.0f, 0.0f, player));
    containerA->AddItem(new EnergyDrink(0.0f, 0.0f, player));
    containerA->AddItem(new EnergyDrink(0.0f, 0.0f, player));

    zone->AddContainer(containerA);
    zone->AddInteractable(new GeneratorBox(600.0f, 150.0f));
    
    zone->AddItems(new Medkit(200.0f, 200.0f, player));
    zone->AddItems(new EnergyDrink(300.f, 200.0f, player));
    zone->AddItems(new KeyCard(350.0f, 200.0f, player));
    zone->AddItems(new Battery(400.0f, 200.0f, player));
    zone->AddItems(new AmmoBox(450.0f, 200.0f, player));

    return zone;
}

Zone* CreateZoneB(Player& player) {
    Zone* zone = new Zone("Security Room B");
    
    zone->walls.push_back({300, 200, 400, 20});
    
    zone->AddEnemy(new Enemy(400, 300, 40, 40, 100, 100, 200, &player, 
                             Enemy::NormalAction::IDLE, Enemy::PatrolType::HORIZONTAL, 
                             1, 150.0f, 40.0f));
    
    Container* containerB = new Container({500, 400, 50, 50});

    containerB->AddItem(new Medkit(0.0f, 0.0f, player));
    containerB->AddItem(new Medkit(0.0f, 0.0f, player));
    containerB->AddItem(new Medkit(0.0f, 0.0f, player));

    zone->AddItems(new KeyCard(500.0f, 200.0f, player));
    zone->AddItems(new Battery(550.0f, 200.0f, player));
    zone->AddItems(new AmmoBox(600.0f, 200.0f, player));

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

    gObjectiveManager.Clear();

    Objective* activateGenerator = new Objective("Activate Generator", "Restore generator power", ObjectiveType::INTERACT, "generator_box");
    Objective* meetHelmuth = new Objective( "Meet Helmuth", "Talk with Helmuth", ObjectiveType::INTERACT, "helmuth_box");
    Objective* gatherItems = new Objective("Gather Required Items", "Collect 3 MedKits", ObjectiveType::COLLECT_ITEM, "MedKit", 3);
    Objective* exitBuilding = new Objective("Exit from the Building", "Leave the facility", ObjectiveType::INTERACT, "exit_box");

    activateGenerator->Unlock();

    gObjectiveManager.AddObjective(activateGenerator);
    gObjectiveManager.AddObjective(meetHelmuth);
    gObjectiveManager.AddObjective(gatherItems);
    gObjectiveManager.AddObjective(exitBuilding);
}