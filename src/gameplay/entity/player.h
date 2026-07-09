#pragma once
#include "gameplay/entity/entity.h"
#include <vector>

class Player : public Entity{
private:
    float currentSpeed = 0.0f; 
    float stamina;
    bool isRunning;
    bool isClouching;

public:
    Player(float x, float y, float width, float height, float health, float speed, float stamina);
    virtual ~Player();

    void Update(float dt) override;
    void Draw() override;

    void Move() override;
    void Move(Vector2 velocity, float dt, const std::vector<Rectangle>& walls);
    void Attack() override;

    void HandleMovement(float dt, const std::vector<Rectangle>& walls);
    void Interact();

    void SetHealth(float h) { this->health = h; }
    void SetStamina(float s) { this->stamina = s; }

    //Getters
    float GetCurrentSpeed() const { return currentSpeed; }
    float GetStamina() const { return stamina; }
    bool CheckIfRunning() const { return isRunning; }
    bool CheckIfClouching() const { return isClouching; }
};