#pragma once

#include "gameplay/interactable/interactable.h"
#include "gameplay/entity/player.h"
#include "item.h"

class Medkit : public Item {
private:
    float healAmount = 30.0f;

public:
    Medkit(float x, float y, Player& player)
        : Item(x, y, 20.0f, 20.0f, "Interact", "MedKit", player){}

    bool ApplyEffect(Player& player) override {

        if (player.GetHealth() >= 100.0f) {
            return false;
        }

        player.Heal(30.0f);
        return true;
    }

    void Draw() override{
        if(!isActivated){
            DrawRectangleRec(bounds, RED);
            DrawRectangle(bounds.x + 8, bounds.y + 3, 4, 14, WHITE);
            DrawRectangle(bounds.x + 3, bounds.y + 8, 14, 4, WHITE);
        }
    }
};

class EnergyDrink : public Item {
public:
    EnergyDrink(float x, float y, Player& player)
        : Item(x, y, 15.0f, 25.0f, "Interact", "Energy Drink", player) {}

    bool ApplyEffect(Player& player) override {

        if (player.GetStamina() >= 100.0f) {
            return false;
        }

        player.SetStamina(100.0f);
        return true;
    }
    
    void Draw() override {
        if(!isActivated){
            DrawRectangleRec(bounds, ORANGE);
            DrawRectangle(bounds.x + 4, bounds.y, 7, bounds.height, BLUE);
        }
    }
};