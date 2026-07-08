#pragma once
#include "gameplay/entity/entity.h"
#include <raymath.h>
#include <vector>

class Player;

enum class EnemyState{
    NORMAL,
    CHASE,
    ATTACK,
    SEARCH,
    RETURN
};

class Enemy : public Entity{
public:
    enum class NormalAction {IDLE, PATROL};
    enum class PatrolType {HORIZONTAL, VERTICAL};

    Enemy(float x, float y, float width, float height, 
        float health, float speed, float sensing_bar, Player* player,
        NormalAction initialAction, PatrolType patrolType, int initialDirection, float customPatrolRange, float customDamage);
    virtual ~Enemy();

    void Update(float dt) override;
    void Update(float dt, const std::vector<Rectangle>& walls);
    void Draw() override;

    void Move() override;
    void Attack() override;

    //SFM Algorithm for enemy to sense the player
    void SensePlayer(Player &player, const std::vector<Rectangle>& walls);
    void ChasePlayer(Player &player, float dt, const std::vector<Rectangle>& walls);

    void DoIdleAction(float dt);
    void DoPatrolAction(float dt, const std::vector<Rectangle>& walls);

    void ReturnToSpawn(float dt, const std::vector<Rectangle>& walls);

    //Getters
    float GetSensing() const { return searchTimer; }
    EnemyState GetCurrentState() const { return currentState; }
    bool GetAttackStatus() { return isAttacked; }

private:
    EnemyState currentState = EnemyState::NORMAL;
    Player* targetPlayer = nullptr;

    float detectionRadius = 200.0f; // Radius within which the enemy can detect the player
    float chase_speed = 1.0f;

    float searchTimer = 0.0f;
    float maxSearchTime = 3.0f;

    NormalAction currentAction;
    PatrolType currentPatrolType;

    float actionTimer = 0.0f;
    float actionDuration = 3.0f;

    Vector2 spawnPoint;
    float patrolRange = 150.0f;
    int moveDirection = 1;

    float attackRadius = 40.0f;
    float attackCoolDown = 1.5f;
    float attackTimer = 0.0f;
    float damage;

    bool isAttacked = false;
};