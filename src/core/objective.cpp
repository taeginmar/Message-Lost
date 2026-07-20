#include "objective.h"

Objective::Objective(const std::string& title, const std::string& description, ObjectiveType type, const std::string& targetId, int requiredProgress)
{
    this->title = title;
    this->description = description;
    this->type = type;
    this->targetId = targetId;
    this->requiredProgress = requiredProgress;
    this->currentProgress = 0;
    this->completed = false;
    this->unlocked = false;
}

Objective::~Objective()
{
}

void Objective::ProcessEvent(ObjectiveType eventType, const std::string& eventId)
{
    if(completed) { return; }
    if(!unlocked) { return; }
    if(type != eventType) { return; }
    if(targetId != eventId) { return; }

    currentProgress++;

    if(currentProgress >= requiredProgress){
        currentProgress = requiredProgress;
        completed = true;
    }
}

void Objective::SetCurrentProgress(int progress) {
    if (!unlocked || completed) return;
    currentProgress = progress;
    if (currentProgress >= requiredProgress) {
        currentProgress = requiredProgress;
        completed = true;
    }
}

bool Objective::IsCompleted() const { return completed; }
bool Objective::IsUnlocked() const { return unlocked; }
void Objective::Unlock(){ unlocked = true; }

const std::string& Objective::GetTitle() const{ return title; }
const std::string& Objective::GetDescription() const { return description; }

int Objective::GetCurrentProgress() const{ return currentProgress; }
int Objective::GetRequiredProgress() const{ return requiredProgress;}

ObjectiveType Objective::GetType() const{ return type;}
const std::string& Objective::GetTargetId() const{return targetId;}