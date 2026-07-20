#pragma once

#include <string>

enum class ObjectiveEventType{
    NONE,
    OBJECTIVE_INTERACT,
    ENEMY_KILLED,
    ITEM_COLLECTED,
    AREA_REACHED,
    DIALOGUE_COMPLETE,
    CUTSCENE_COMPLETE,
    PUZZLE_COMPLETE
};

struct ObjectiveEvent{
    ObjectiveEventType type;
    std::string targetID;
    int amount;

    ObjectiveEvent(){
        type = ObjectiveEventType::NONE;
        targetID = "";
        amount = 1;
    }


    ObjectiveEvent(
        ObjectiveEventType eventType,
        const std::string& id,
        int value = 1
    ){
        type = eventType;
        targetID = id;
        amount = value;
    }
};