#pragma once
#include <vector>
#include "objective.h"

class ObjectiveManager{
private:
    std::vector<Objective*> objectives;

public:
    ObjectiveManager();
    ~ObjectiveManager();

    void AddObjective(Objective* objective);
    void ReportEvent(ObjectiveType eventType,const std::string& eventId);
    void Clear();

    const std::vector<Objective*>& GetObjectives() const;
};