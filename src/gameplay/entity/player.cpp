#include "player.h"

Player::Player(float x, float y, float width, float height, float health, float speed, float stamina)
    :Entity(x, y, width, height, health, speed), stamina(stamina), isRunning(false), isClouching(false) {}

Player::~Player() {}

void Player::Update(float dt){
    if(!isRunning && stamina < 100.0f){
        stamina += 5.0f * dt;
    }
}

void Player::Draw(){
    DrawRectangleRec(bounds, BLUE);
}

void Player::Move(){
    // This function is intentionally left empty as movement is handled in HandleMovement
}

void Player::Move(Vector2 velocity, float dt, const std::vector<Rectangle>& walls){
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
    Vector2 velocity = {0.0f, 0.0f};

    if(IsKeyDown(KEY_W)) velocity.y = -1.0f;
    if(IsKeyDown(KEY_S)) velocity.y = 1.0f;
    if(IsKeyDown(KEY_A)) velocity.x = -1.0f;
    if(IsKeyDown(KEY_D)) velocity.x = 1.0f;

    currentSpeed = speed;
    if(IsKeyDown(KEY_LEFT_SHIFT) && stamina > 0){
        currentSpeed = speed * 1.5f;
        isRunning = true;
        stamina -= 100.0f * dt;
    }
    else{
        isRunning = false;
        if(stamina < 100.0f) stamina += 5.0f * dt;
    }

    Vector2 final_vel = { velocity.x * currentSpeed, velocity.y * currentSpeed };
    Move(final_vel, dt, walls);
}

void Player::Attack(){

}

void Player::Interact(){

}