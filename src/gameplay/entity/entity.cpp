#include "entity.h"

Entity::Entity(float x, float y, float width, float height, float health, float speed) 
    : GameObject(x, y, width, height), health(health), speed(speed)
{
    this->isAlive = (this->health > 0);
    this->maxHealth = health;
}

Entity::~Entity(){}

void Entity::TakeDamage(float amount){
    if(!isAlive) return;

    health -= amount;

    if(health <= 0){
        health = 0;
        isAlive = false;
    }
}