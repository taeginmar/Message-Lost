#include "levels/Mock_Up_Level/mock_zones.h"

static Objective* gObjMeetHelmuth = nullptr;
static Objective* gObjGatherItems = nullptr;
static Objective* gObjKillEnemies = nullptr;
static Objective* gObjExitBuilding = nullptr;

extern Player* gPlayer;
static Zone* gZoneA = nullptr;
static bool gEnemiesSpawned = false;

// Objective 1
GeneratorBox::GeneratorBox(float x, float y)
    : Interactable(x, y, 40.0f, 40.0f, "Press E to Activate Generator"), isActivated(false) {}

void GeneratorBox::OnInteract() {
    if (!isActivated) {
        isActivated = true;
        gObjectiveManager.ReportEvent(ObjectiveType::INTERACT, "generator_box");
    }
}

void GeneratorBox::Draw() {
    Color boxColor = isActivated ? GREEN : RED;
    DrawRectangleRec(bounds, boxColor);
    DrawText("GEN", bounds.x + 5, bounds.y + 15, 12, WHITE);
}

// Objective 2
MeetHelmuth::MeetHelmuth(float x, float y, Player& player, Zone* zoneA)
    : Interactable(x, y, 40.0f, 40.0f, "Press E to Talk with Helmuth"), isTalked(false), targetPlayer(player), targetZoneA(zoneA) {}

void MeetHelmuth::OnInteract() {
    if (!isTalked) {
        isTalked = true;
        gObjectiveManager.ReportEvent(ObjectiveType::DIALOGUE_COMPLETE, "helmuth_box");
    }
}

void MeetHelmuth::Draw() {
    Color npcColor = isTalked ? BLUE : YELLOW;
    DrawRectangleRec(bounds, npcColor);
    DrawText("NPC", bounds.x + 5, bounds.y + 15, 12, BLACK);
}

// Objective 4
EnemyNeutralize::EnemyNeutralize(float x, float y, Player* player)
    : Enemy(x, y, 40.0f, 40.0f, 100.0f, 100.0f, 200.0f, player,
            Enemy::NormalAction::IDLE, Enemy::PatrolType::HORIZONTAL,
            1, 150.0f, 40.0f), eventReported(false) {}

void EnemyNeutralize::OnDeath(){
    if(!eventReported && gObjKillEnemies->IsUnlocked()){
        eventReported = true;
        TraceLog(LOG_INFO,"Enemy Dead");
        gObjectiveManager.ReportEvent(ObjectiveType::KILL,"enemy_guard");
    }
}

void EnemyNeutralize::Draw() {
    if (!IsDead()) {
        DrawRectangleRec(bounds, RED);
        DrawText("ENM", (int)bounds.x + 5, (int)bounds.y + 15, 12, WHITE);
    }
}

// Objective 5
FindExit::FindExit(float x, float y)
    : Interactable(x, y, 50.0f, 50.0f, "Press E to Exit Building") {}

void FindExit::OnInteract(){
    if(!gObjExitBuilding->IsUnlocked())
        return;

    gObjectiveManager.ReportEvent(ObjectiveType::REACH_AREA,"exit_box");
}

void FindExit::Draw() {
    DrawRectangleRec(bounds, DARKPURPLE);
    DrawText("EXIT", bounds.x + 10, bounds.y + 18, 10, WHITE);
}

// Set Up Level Zone
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

Zone* CreateZoneB(Player& player, Zone* zoneA) {
    Zone* zone = new Zone("Security Room B");
    
    zone->walls.push_back({300, 200, 400, 20});
    
    Container* containerB = new Container({500, 400, 50, 50});
    containerB->AddItem(new Medkit(0.0f, 0.0f, player));
    containerB->AddItem(new Medkit(0.0f, 0.0f, player));
    containerB->AddItem(new Medkit(0.0f, 0.0f, player));

    zone->AddItems(new KeyCard(500.0f, 200.0f, player));
    zone->AddItems(new Battery(550.0f, 200.0f, player));
    zone->AddItems(new AmmoBox(600.0f, 200.0f, player));

    zone->AddContainer(containerB);

    zone->AddInteractable(new MeetHelmuth(350.0f, 120.0f, player, zoneA));
    zone->AddInteractable(new FindExit(700.0f, 450.0f));
    
    return zone;
}

// Set Up Level Flow
void SetUpMockUpLevel(Level& level, Player& player) {
    gPlayer = &player;
    gEnemiesSpawned = false;

    Zone* zoneA = CreateZoneA(player);
    Zone* zoneB = CreateZoneB(player, zoneA);

    gZoneA = zoneA;

    level.AddZone(zoneA);
    level.AddZone(zoneB);

    level.AddDoor(Door({750, 250, 50, 100}, 1, {100, 300}, 0)); 
    level.AddDoor(Door({20, 250, 50, 100}, 0, {400, 300}, 1));

    gObjectiveManager.Clear();

    Objective* activateGenerator = new Objective("Activate Generator", "Restore generator power", ObjectiveType::INTERACT, "generator_box");
    gObjMeetHelmuth = new Objective("Meet Helmuth", "Talk with Helmuth", ObjectiveType::DIALOGUE_COMPLETE, "helmuth_box");
    gObjGatherItems = new Objective("Gather Required Items", "Collect 3 MedKits", ObjectiveType::COLLECT_ITEM, "medkit", 3);
    gObjKillEnemies = new Objective("Clear the Corridor", "Eliminate Enemies", ObjectiveType::KILL, "enemy_guard", 2);
    gObjExitBuilding = new Objective("Exit from the Building", "Leave the facility", ObjectiveType::REACH_AREA, "exit_box");

    gObjectiveManager.AddObjective(activateGenerator);
    gObjectiveManager.AddObjective(gObjMeetHelmuth);
    gObjectiveManager.AddObjective(gObjGatherItems);
    gObjectiveManager.AddObjective(gObjKillEnemies);
    gObjectiveManager.AddObjective(gObjExitBuilding);

    gObjectiveManager.SetObjectiveUnlockedCallback([](Objective* objective){
        if(objective->GetTitle() != "Clear the Corridor") return;
        
        TraceLog(LOG_INFO,"Spawn Enemy");
        gZoneA->AddEnemy(new EnemyNeutralize(400,300,gPlayer));
        gZoneA->AddEnemy(new EnemyNeutralize(550, 300,gPlayer));
        }
    );
}