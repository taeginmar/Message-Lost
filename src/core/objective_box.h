#pragma once
#include <string>

#include "gameplay/interactable/interactable.h"
#include "objective_manager.h"

class ObjectiveBox : public Interactable {
private:
  std::string objectiveID;
  ObjectiveManager *objectiveManager;

public:
  ObjectiveBox(float x, float y, float width, float height,
               const std::string &id, ObjectiveManager *manager);

  ~ObjectiveBox();

  void OnInteract() override;
  void Draw() override;
};