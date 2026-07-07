#pragma once
#include "gameplay/entity/entity.h"

class Player;
class Enemy : public Entity{
private:
    float sensing_bar;
    int currentState;
public:
    Enemy(float x, float y, float width, float height, float health, float speed, float sensing_bar);
    virtual ~Enemy();

    void Update(float dt) override;
    void Draw() override;

    void Move() override;
    void Attack() override;

    void SensePlayer(Player &player);
    
    //Getters
    float GetSensing() const { return sensing_bar; }
    int GetCurrentState() const { return currentState; }
};