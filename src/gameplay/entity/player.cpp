#include "player.h"
#include "gameplay/inventory/inventory.h"

Player::Player(float x, float y, float width, float height, float health, float speed, float stamina)
    :Entity(x, y, width, height, health, speed), stamina(stamina), isRunning(false), isClouching(false) {
    counterRange = 80.0f;
    counterDamage = 25.0f;
    counterStaminaCost = 70.0f;

    inventory = new Inventory(16, 4);
    inventory->UnlockSlots(5);

    this->stamina = stamina;
    this->maxHealth = health;
    this->spawnPoint = { x, y };
}

Player::~Player() {
    delete inventory;
}

void Player::Update(float dt){
    if(isDead) return;

    if(health <= 0.0f){
        health = 0.0f;
        Die();
        return;
    }

    if(!isRunning && stamina < 100.0f){
        stamina += 3.0f * dt;
    }

}

void Player::Draw(){
    DrawRectangleRec(bounds, BLUE);

    if(inventory && inventory->IsOpen()){
        inventory->Draw(*this);
    }
}

// This function is intentionally left empty as movement is handled in HandleMovement
void Player::Move(){}

void Player::Move(Vector2 velocity, float dt, const std::vector<Rectangle>& walls){
    if(isDead) return;

    bounds.x += velocity.x * dt;
    for(const auto& wall : walls){
        if(CheckCollisionRecs(bounds, wall)){
            bounds.x -= velocity.x * dt;
            break;
        }
    }

    bounds.y += velocity.y * dt;
    for(const auto& wall : walls){
        if(CheckCollisionRecs(bounds, wall)){
            bounds.y -= velocity.y * dt;
            break;
        }
    }
}

void Player::HandleMovement(float dt, const std::vector<Rectangle>& walls){
    if(isDead) return;
    
    Vector2 velocity = {0.0f, 0.0f};

    if(IsKeyDown(KEY_W)) velocity.y = -1.0f;
    if(IsKeyDown(KEY_S)) velocity.y = 1.0f;
    if(IsKeyDown(KEY_A)) velocity.x = -1.0f;
    if(IsKeyDown(KEY_D)) velocity.x = 1.0f;

    currentSpeed = speed;
    if(IsKeyDown(KEY_LEFT_SHIFT) && stamina > 0){
        currentSpeed = speed * 2.0f;
        isRunning = true;
        stamina -= 50.0f * dt;
    }
    else{
        isRunning = false;
        if(stamina < 100.0f) stamina += 10.0f * dt;
    }

    Vector2 final_vel = { velocity.x * currentSpeed, velocity.y * currentSpeed };
    Move(final_vel, dt, walls);
}

void Player::Attack(){}

void Player::Attack(std::vector<Enemy*>& enemies){
    if(isDead) return;
    
    if(!IsKeyPressed(KEY_F))
        return;

    Rectangle playerBounds = GetBounds();

    Vector2 playerCenter = {
        playerBounds.x + playerBounds.width * 0.5f,
        playerBounds.y + playerBounds.height * 0.5f
    };

    for(auto* enemy : enemies){
        if(enemy == nullptr)
            continue;

        Rectangle enemyBounds = enemy->GetBounds();

        Vector2 enemyCenter = {
            enemyBounds.x + enemyBounds.width * 0.5f,
            enemyBounds.y + enemyBounds.height * 0.5f
        };

        float distance = Vector2Distance(playerCenter, enemyCenter);

        if(distance > counterRange)
            continue;

        if(stamina < counterStaminaCost)
            return;

        Vector2 attackDirection = {
            enemyCenter.x - playerCenter.x,
            enemyCenter.y - playerCenter.y
        };

        attackDirection = Vector2Normalize(attackDirection);

        float enemyHpBefore = enemy->GetHealth();

        enemy->TakeCounterAttack(
            counterDamage,
            attackDirection
        );

        float enemyHpAfter = enemy->GetHealth();
        float damageDealt = enemyHpBefore - enemyHpAfter;

        stamina -= counterStaminaCost;

        TraceLog(LOG_INFO, TextFormat("Counter Attack Success! Damage: %.0f | Enemy HP: %.0f -> %.0f", damageDealt, enemyHpBefore, enemyHpAfter));
    }
}

void Player::SetHealth(float h){
    this->health = h;

    if(this->health > 0.0f){
        isAlive = true;
    }  
}

void Player::Die(){
    isDead = true;
    isAlive = false;   
}

void Player::Respawn(){
    health = maxHealth;
    stamina = 100.0f;

    bounds.x = spawnPoint.x;
    bounds.y = spawnPoint.y;

    isRunning = false;
    isClouching = false;

    isDead = false;
    isAlive = true;
}

void Player::ClearInventory(){
    if(inventory){
        inventory->Clear();
    }
}

void Player::Interact(){ if(isDead) return; }