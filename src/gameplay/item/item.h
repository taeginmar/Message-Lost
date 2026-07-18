#pragma once

#include "gameplay/interactable/interactable.h"
#include "gameplay/entity/player.h"
#include <string>

enum ItemType { ITEM_CONSUMABLE, ITEM_KEY, ITEM_QUEST, ITEM_WEAPON };

class Item : public Interactable{
protected:
    Player& targetPlayer;
    std::string itemType;

    int stackCount = 1;

public:
    Item(float x, float y, float width, float heigth, std::string prompText, std::string type, Player& player)
    : Interactable(x, y, width, heigth, prompText), itemType(type), targetPlayer(player){}

    virtual ~Item() {}

    void OnInteract() override {
        isActivated = true;
    }

    virtual bool ApplyEffect(Player& player) = 0;

    void Draw(){
        if(!isActivated){
            Interactable::Draw();
        }
    }

    std::string GetItemType() const { return itemType; }
    int GetStackCount() const { return stackCount; }
    void SetStackCount(int count) { stackCount = count; }
};