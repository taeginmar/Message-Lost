#pragma once

#include "objective.h"
#include <vector>
#include <string>

class ObjectiveManager {
public:
    ObjectiveManager();
    ~ObjectiveManager();

    void Clear();
    void AddObjective(Objective* objective);
    void ReportEvent(ObjectiveType eventType, const std::string& eventId, int amount = 1);
    void UpdateProgress(ObjectiveType eventType, const std::string& eventId, int currentAmount);
    const std::vector<Objective*>& GetObjectives() const;
    Objective* GetCurrentObjective() const;

private:
    std::vector<Objective*> objectives;
};

extern ObjectiveManager gObjectiveManager;