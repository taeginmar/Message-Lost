#pragma once

#include "gameplay/interactable/interactable.h"
#include "gameplay/entity/player.h"
#include "item.h"

class KeyCard : public Item {
public:
    KeyCard(float x, float y, Player& player)
        : Item(x, y, 20.0f, 20.0f, "Interact", "KeyCard", player) {}

    bool ApplyEffect(Player& player) override {
        return false;
    }

    void Draw() override {
        if (!isActivated) {
            DrawRectangleRec(bounds, YELLOW);
        }
    }
};

class Battery : public Item {
public:
    Battery(float x, float y, Player& player)
        : Item(x, y, 20.0f, 20.0f, "Interact", "Battery", player) {}

    bool ApplyEffect(Player& player) override {
        return false;
    }

    void Draw() override {
        if (!isActivated) {
            DrawRectangleRec(bounds, GREEN);
        }
    }
};

class AmmoBox : public Item {
public:
    AmmoBox(float x, float y, Player& player)
        : Item(x, y, 20.0f, 20.0f, "Interact", "AmmoBox", player) {}

    bool ApplyEffect(Player& player) override {
        return false;
    }

    void Draw() override {
        if (!isActivated) {
            DrawRectangleRec(bounds, BROWN);
        }
    }
};