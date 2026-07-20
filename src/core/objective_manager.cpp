#include "objective_manager.h"

ObjectiveManager gObjectiveManager;

ObjectiveManager::ObjectiveManager(){}

ObjectiveManager::~ObjectiveManager(){
    for(Objective* objective : objectives){
        delete objective;
    }

    Clear();
}

void ObjectiveManager::Clear() {
    for(Objective* objective : objectives){
        delete objective;
    }
    objectives.clear();
}

void ObjectiveManager::AddObjective(Objective* objective){
    if(objectives.empty()){
        objective->Unlock();
    }

    objectives.push_back(objective);
}

void ObjectiveManager::ReportEvent(ObjectiveType eventType,const std::string& eventId){
    for(size_t i = 0;i < objectives.size(); i++){
        Objective* objective = objectives[i];

        objective->ProcessEvent(eventType, eventId);

        if(objective->IsCompleted()){
            if(i + 1 < objectives.size()){
                objectives[i + 1]->Unlock();
            }
        }
    }
}

const std::vector<Objective*>& ObjectiveManager::GetObjectives() const {return objectives;}