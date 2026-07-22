#include "objective.h"

Objective::Objective(const std::string& title, const std::string& description, ObjectiveType type, const std::string& targetId, int requiredAmount)
    : title(title), description(description), type(type), targetId(targetId),
      requiredAmount(requiredAmount), currentAmount(0), unlocked(false), completed(false) {}

void Objective::Unlock() {
    unlocked = true;
}

bool Objective::IsUnlocked() const {
    return unlocked;
}

bool Objective::IsCompleted() const {
    return completed;
}

void Objective::ProcessEvent(ObjectiveType eventType, const std::string& eventId, int amount) {
    if (!unlocked || completed) return;

    if (type == eventType && targetId == eventId) {
        currentAmount += amount;
        if (currentAmount >= requiredAmount) {
            completed = true;
        }
    }
}

void Objective::SetCurrentProgress(int progress) {
    if (!unlocked || completed) return;

    currentAmount = progress;
    TraceLog(LOG_INFO,"Objective %s progress %d/%d",title.c_str(),currentAmount, requiredAmount);

    if (currentAmount >= requiredAmount) {
        completed = true;
        TraceLog(LOG_INFO,"%s COMPLETE",title.c_str());
    }
}

std::string Objective::GetTitle() const {
    return title;
}

std::string Objective::GetDescription() const {
    return description;
}

ObjectiveType Objective::GetType() const {
    return type;
}

std::string Objective::GetTargetId() const {
    return targetId;
}