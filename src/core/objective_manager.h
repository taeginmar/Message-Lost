#pragma once

#include "objective.h"
#include <vector>
#include <string>
#include <functional>

class ObjectiveManager {
public:
    ObjectiveManager();
    ~ObjectiveManager();

    void Clear();
    void AddObjective(Objective* objective);
    void ReportEvent(ObjectiveType eventType,const std::string& eventId,int amount = 1);
    void UpdateProgress(ObjectiveType eventType,const std::string& eventId,int currentAmount);

    const std::vector<Objective*>& GetObjectives() const;
    Objective* GetCurrentObjective() const;

    void SetObjectiveUnlockedCallback(
        std::function<void(Objective*)> callback);

private:
    void CompleteObjective(size_t index);
    std::vector<Objective*> objectives;
    std::function<void(Objective*)> objectiveUnlockedCallback;
};

extern ObjectiveManager gObjectiveManager;