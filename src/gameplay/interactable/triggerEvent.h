#pragma once

#include <string>

enum class ObjectiveEventType{
    ENEMY_KILLED,
    ITEM_COLLECTED,
    AREA_REACHED,
    TRIGGER_ACTIVATED
};

enum class ObjectiveState{
    LOCKED,
    ACTIVE,
    COMPLETED
};

struct ObjectiveEvent{
    ObjectiveEventType type;
    std::string targetID;
    int amount = 1;
};

class Objective {
protected:
    std::string id;
    std::string title;
    std::string description;

    int currentProgress;
    int targetProgress;
    ObjectiveState state;

public:
    Objective(
        const std::string& objectiveID,
        const std::string& objectiveTitle,
        const std::string& objectiveDescription,
        int target
    );

    virtual ~Objective();

    virtual void OnEvent(const ObjectiveEvent& event) = 0;

    void Activate();
    void Complete();
    bool IsCompleted() const;
    bool IsActive() const;

    ObjectiveState GetState() const;
    const std::string& GetID() const;
    const std::string& GetTitle() const;
    const std::string& GetDescription() const;
    
    int GetCurrentProgress() const;
    int GetTargetProgress() const;
};

class KillObjective : public Objective{
private:
    std::string enemyType;
public:
    KillObjective(
        const std::string& objectiveID,
        const std::string& objectiveTitle,
        const std::string& objectiveDescription,
        const std::string& enemyID,
        int targetCount
    );

    void OnEvent(const ObjectiveEvent& event) override;
};

class CollectObjective : public Objective{
private:
    std::string itemType;
public:
    CollectObjective(
        const std::string& objectiveID,
        const std::string& objectiveTitle,
        const std::string& objectiveDescription,
        const std::string& itemID,
        int targetCount
    );

    void OnEvent(const ObjectiveEvent& event) override;
};

class ReachAreaObjective : public Objective{
private:
    std::string areaID;
public:

    ReachAreaObjective(
        const std::string& objectiveID,
        const std::string& objectiveTitle,
        const std::string& objectiveDescription,
        const std::string& targetArea
    );

    void OnEvent(const ObjectiveEvent& event) override;
};

class TriggerObjective : public Objective{
private:
    std::string triggerID;
public:

    TriggerObjective(
        const std::string& objectiveID,
        const std::string& objectiveTitle,
        const std::string& objectiveDescription,
        const std::string& targetTrigger
    );

    void OnEvent(const ObjectiveEvent& event) override;
};  