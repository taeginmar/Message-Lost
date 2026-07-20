#pragma once
#include <string>

enum class ObjectiveType {
    INTERACT,
    COLLECT_ITEM,
    REACH_AREA,
    DIALOGUE_COMPLETE
};

class Objective {
private:
    std::string title;
    std::string description;
    ObjectiveType type;
    std::string targetId;
    int currentProgress;
    int requiredProgress;
    bool completed;
    bool unlocked;

public:
    Objective(const std::string& title, const std::string& description, ObjectiveType type, const std::string& targetId, int requiredProgress = 1);
    virtual ~Objective();

    virtual void ProcessEvent(ObjectiveType eventType, const std::string& eventId);

    bool IsCompleted() const;
    bool IsUnlocked() const;
    void Unlock();

    const std::string& GetTitle() const;
    const std::string& GetDescription() const;
    int GetCurrentProgress() const;
    int GetRequiredProgress() const;
    ObjectiveType GetType() const;
    const std::string& GetTargetId() const;
    void SetCurrentProgress(int progress);
};