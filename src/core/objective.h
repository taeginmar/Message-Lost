#pragma once
#include <string>

enum class ObjectiveType {
    INTERACT,
    DIALOGUE_COMPLETE,
    COLLECT_ITEM,
    KILL,
    REACH_AREA
};

class Objective {
public:
    Objective(const std::string& title, const std::string& description, ObjectiveType type, const std::string& targetId = "", int requiredAmount = 1);

    void Unlock();
    bool IsUnlocked() const;
    bool IsCompleted() const;
    void ProcessEvent(ObjectiveType eventType, const std::string& eventId, int amount = 1);
    void SetCurrentProgress(int progress);

    std::string GetTitle() const;
    std::string GetDescription() const;
    ObjectiveType GetType() const;
    std::string GetTargetId() const;

private:
    std::string title;
    std::string description;
    ObjectiveType type;
    std::string targetId;
    int requiredAmount;
    int currentAmount;
    bool unlocked;
    bool completed;
};