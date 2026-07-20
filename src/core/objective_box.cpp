#include "objective_box.h"
#include "objective_event.h"
ObjectiveBox::ObjectiveBox(
    float x,
    float y,
    float width,
    float height,
    const std::string& id,
    ObjectiveManager* manager
): Interactable(x,y,width,height,"Press E to interact"){
    objectiveID = id;
    objectiveManager = manager;
}

ObjectiveBox::~ObjectiveBox(){}

void ObjectiveBox::OnInteract(){
    if(isActivated)return;

    isActivated = true;

    if(objectiveManager == nullptr)return;
    
    objectiveManager->ReportEvent(ObjectiveType::INTERACT, objectiveID);
    prompt = "Completed";
}

void ObjectiveBox::Draw(){
    Color boxColor = BLUE;

    if(isActivated){
        boxColor = GRAY;
    }

    DrawRectangleRec(bounds,boxColor);
    DrawText("OBJ",bounds.x + 10,bounds.y + 10,10,WHITE);
}