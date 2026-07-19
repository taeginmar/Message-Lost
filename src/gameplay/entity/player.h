#pragma once

#include <vector>
#include "gameplay/entity/enemy.h"

class Inventory;
class Enemy;

class Player : public Entity{
private:
    float currentSpeed = 0.0f; 
    float stamina;
    bool isRunning;
    bool isClouching;

    Inventory* inventory;

    float counterRange;
    float counterDamage;
    float counterStaminaCost;

public:
    Player(float x, float y, float width, float height, float health, float speed, float stamina);
    virtual ~Player();

    std::vector<Enemy*>* allEnemies = nullptr; 

    void SetEnemyReference(std::vector<Enemy*>* enemies) { allEnemies = enemies; }

    void Update(float dt) override;
    void Draw() override;

    void Move() override;
    void Move(Vector2 velocity, float dt, const std::vector<Rectangle>& walls);
    void Attack() override;
    void Attack(std::vector<Enemy*>& enemies);

    void HandleMovement(float dt, const std::vector<Rectangle>& walls);
    void Interact();

    void SetHealth(float h) { this->health = h; }
    void SetStamina(float s) { this->stamina = s; }

    void Heal(float amount) {
        health += amount;

        if (health > 100.0f) {
            health = 100.0f;
        }
    }

    void RestoreStamina(float amount) {
        this->stamina += amount;
        if (this->stamina > 100.0f) this->stamina = 100.0f; // ล็อกไม่ให้สเตมิน่าเกิน 100 (หรือปรับตามมินิมัมเกมคุณ)
    }

    Inventory* GetInventory() const { return inventory; }

    //Getters
    float GetCurrentSpeed() const { return currentSpeed; }
    float GetStamina() const { return stamina; }
    bool CheckIfRunning() const { return isRunning; }
    bool CheckIfClouching() const { return isClouching; }

    float GetCounterRange() const { return counterRange; }
    float GetCounterDamage() const { return counterDamage; }
    float GetCounterStaminaCost() const { return counterStaminaCost; }
};