#pragma once

#include "gameplay/interactable/interactable.h"
#include "gameplay/entity/player.h"
#include <string>

enum ItemType { ITEM_CONSUMABLE, ITEM_KEY, ITEM_QUEST, ITEM_WEAPON };

class Item : public Interactable{
protected:
    Player& targetPlayer;
    std::string itemType;

public:
    Item(float x, float y, float width, float heigth, std::string prompText, std::string type, Player& player)
    : Interactable(x, y, width, heigth, prompText), itemType(type), targetPlayer(player){}

    virtual ~Item() {}

    void OnInteract() override {
        if(!isActivated){
            isActivated = true;
            ApplyEffect();
        }
    }

    virtual void ApplyEffect() = 0;

    void Draw(){
        if(!isActivated){
            Interactable::Draw();
        }
    }

    std::string GetItemType() const { return itemType; }
};