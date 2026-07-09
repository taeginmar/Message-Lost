#pragma once

#include "gameplay/interactable/interactable.h"
#include "gameplay/entity/player.h"

class Item : public Interactable{
protected:
    Player& targetPlayer;

public:
    Item(float x, float y, float width, float heigth, std::string prompText, Player& player)
    : Interactable(x, y, width, heigth, prompText), targetPlayer(player){}

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
};