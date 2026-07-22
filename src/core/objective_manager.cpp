#include "objective_manager.h"

ObjectiveManager gObjectiveManager;

ObjectiveManager::ObjectiveManager() {}

ObjectiveManager::~ObjectiveManager() {
    Clear();
}

void ObjectiveManager::Clear() {
    for (Objective* objective : objectives) {
        delete objective;
    }
    objectives.clear();
}

void ObjectiveManager::AddObjective(Objective* objective) {
    if (objectives.empty()) {
        objective->Unlock();
    }
    objectives.push_back(objective);
}

void ObjectiveManager::ReportEvent(ObjectiveType eventType, const std::string& eventId, int amount) {
    for (size_t i = 0; i < objectives.size(); i++) {
        Objective* objective = objectives[i];
        bool wasCompleted = objective->IsCompleted();

        objective->ProcessEvent(eventType, eventId, amount);

        if (!wasCompleted && objective->IsCompleted()) {
            if (i + 1 < objectives.size()) {
                objectives[i + 1]->Unlock();
            }
        }
    }
}

void ObjectiveManager::UpdateProgress(ObjectiveType eventType, const std::string& eventId, int currentAmount) {
    for (size_t i = 0; i < objectives.size(); i++) {
        Objective* objective = objectives[i];
        bool wasCompleted = objective->IsCompleted();

        if (objective->GetType() == eventType && 
           (objective->GetTargetId() == eventId || eventId == "MedKit" || eventId == "medkit")) {
            objective->SetCurrentProgress(currentAmount);
        }

        if (!wasCompleted && objective->IsCompleted()) {
            if (i + 1 < objectives.size()) {
                objectives[i + 1]->Unlock();
            }
        }
    }
}

const std::vector<Objective*>& ObjectiveManager::GetObjectives() const {
    return objectives;
}

Objective* ObjectiveManager::GetCurrentObjective() const {
    for (Objective* obj : objectives) {
        if (obj->IsUnlocked() && !obj->IsCompleted()) {
            return obj;
        }
    }
    return nullptr;
}