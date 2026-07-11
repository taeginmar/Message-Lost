#pragma once
#include "gameplay/game_object/game_object.h"

class Entity : public GameObject{
protected:
    float health;
    float maxHealth;
    float speed;
    bool isAlive;
public:
    Entity(float x, float y, float width, float height, float hp, float spd);
    virtual ~Entity();

    virtual void Update(float dt) override = 0;
    virtual void Draw() override = 0;

    virtual void Move() = 0;
    virtual void Attack() = 0;

    virtual void TakeDamage(float amount);

    //Getters
    float GetMaxHealth() const { return maxHealth; }
    float GetHealth() const { return health; }
    float GetSpeed() const { return speed; }
    bool IsAlive() const { return isAlive; }
};