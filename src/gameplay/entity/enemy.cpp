#include "enemy.h"

Enemy::Enemy(float x, float y, float width, float height, float health, float speed, float sensing_bar)
    : Entity(x, y, width, height, health, speed), sensing_bar(sensing_bar), currentState(0){}

Enemy::~Enemy() {}

float patrolDirection = 1.0f;
float patrolTimer = 0.0f;

void Enemy::Update(float dt){
    Move();

    patrolTimer += dt;
    if(patrolTimer >= 2.0f){
        patrolDirection *= -1.0f;
        patrolTimer = 0.0f;
    }

    bounds.x += patrolDirection * speed * dt;
}

void Enemy::Draw(){
    DrawRectangleRec(bounds, GRAY);
    DrawRectangleLinesEx(bounds, 1, YELLOW);
}

void Enemy::Move(){

}

void Enemy::Attack(){

}

void Enemy::SensePlayer(Player &player){

}