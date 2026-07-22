#include "enemy.h"
#include "player.h"

//helper for collision
void MoveWithCollision(Rectangle& bounds, float moveX, float moveY, const std::vector<Rectangle>& walls){
    bounds.x += moveX;
    for(const auto& wall : walls){
        if(CheckCollisionRecs(bounds, wall)){
            bounds.x -= moveX;
            break;
        }
    }

    bounds.y += moveY;
    for(const auto& wall : walls){
        if(CheckCollisionRecs(bounds, wall)){
            bounds.y -= moveY;
            break;
        }
    }
}

bool CheckLineRectCollision(Vector2 p1, Vector2 p2, Rectangle rec) {
    Vector2 topLeft = { rec.x, rec.y };
    Vector2 topRight = { rec.x + rec.width, rec.y };
    Vector2 bottomLeft = { rec.x, rec.y + rec.height };
    Vector2 bottomRight = { rec.x + rec.width, rec.y + rec.height };
    Vector2 intersection; 

    if (CheckCollisionLines(p1, p2, topLeft, topRight, &intersection)) return true;    // ด้านบน
    if (CheckCollisionLines(p1, p2, bottomLeft, bottomRight, &intersection)) return true; // ด้านล่าง
    if (CheckCollisionLines(p1, p2, topLeft, bottomLeft, &intersection)) return true;   // ด้านซ้าย
    if (CheckCollisionLines(p1, p2, topRight, bottomRight, &intersection)) return true; // ด้านขวา

    return false;
}

Enemy:: Enemy(float x, float y, float width, float height, 
        float health, float speed, float sensing_bar, Player* player,
        NormalAction initialAction, PatrolType patrolType, int initialDirection, float customPatrolRange, float customDamage)
    : Entity(x, y, width, height, health, speed), targetPlayer(player)
{
    this->chase_speed = this->speed * 2.0f;
    this->spawnPoint = { x, y };

    this->currentAction = initialAction;
    this->currentPatrolType = patrolType;
    this->moveDirection = initialDirection;
    this->patrolRange = customPatrolRange;
    this->damage = customDamage;

    this->health = health;
    this->isAlive = true;
}

Enemy::~Enemy() {}

void Enemy::Update(float dt){
    std::vector<Rectangle> emptyWalls;
    Update(dt, emptyWalls);
}

void Enemy::Update(float dt, const std::vector<Rectangle>& walls){
    if(IsDead()){
        if(!deathHandled){
            deathHandled = true;
            OnDeath();
        }
        return;
    }

    deathHandled = false;
    
    if(targetPlayer == nullptr) return;

    if(stunTimer > 0.0f){
        stunTimer -= dt;

        if(stunTimer <= 0.0f){
            stunTimer = 0.0f;
            isStunned = false;
        }
        return;
    }

    if(attackTimer > 0.0f){
        attackTimer -= dt;
    }

    SensePlayer(*targetPlayer, walls);

    switch(currentState){
        case EnemyState::NORMAL:
            if(currentAction == NormalAction::IDLE)
                DoIdleAction(dt);
            else if(currentAction == NormalAction::PATROL)
                DoPatrolAction(dt, walls);
            break;

        case EnemyState::CHASE:
            ChasePlayer(*targetPlayer, dt, walls);
            break;

        case EnemyState::ATTACK:
            Attack();
            break;

        case EnemyState::RETURN:
            ReturnToSpawn(dt, walls);
            break;

        case EnemyState::SEARCH:
            break;
    }
}

void Enemy::Draw(){
    if(IsDead()){
        DrawRectangleRec(bounds, PURPLE);
        return;
    }

    if(currentState == EnemyState::NORMAL && currentAction == NormalAction::IDLE){
        float rotation = actionTimer * 90.0f;
        Vector2 origin = { bounds.width/2, bounds.height/2 };
        Rectangle drawbounds { bounds.x + origin.x, bounds.y + origin.y, bounds.width, bounds.height };
        DrawRectanglePro(drawbounds, origin, rotation, RED);
    }
    else{
        DrawRectangleRec(bounds, RED);
    }

    if(targetPlayer != nullptr){
        Vector2 enemyCenter = { bounds.x + bounds.width/2, bounds.y + bounds.height/2 };
        Rectangle pBounds = targetPlayer->GetBounds();
        Vector2 playerCenter = { pBounds.x + pBounds.width/2, pBounds.y + pBounds.height/2 };

        Color lineVisualColor = (currentState == EnemyState::CHASE) ? GREEN : GRAY;
        DrawLineV(enemyCenter, playerCenter, lineVisualColor);
    }
}

void Enemy::Move(){}
void Enemy::Attack(){
    if(attackTimer <= 0.0f){
        targetPlayer->TakeDamage(this->damage);
        TraceLog(LOG_INFO, "Enemy ATTACKED!");

        attackTimer = attackCoolDown;

        isAttacked = true;

        currentState = EnemyState::CHASE;
    }
}

void Enemy::SensePlayer(Player &player, const std::vector<Rectangle>& walls){
    Vector2 enemyCenter = { bounds.x + bounds.width/2, bounds.y + bounds.height/2};

    Rectangle playerBounds = player.GetBounds();
    Vector2 playerCenter = { playerBounds.x + playerBounds.width/2, playerBounds.y + playerBounds.height/2};

    float distance = Vector2Distance(enemyCenter, playerCenter);

    if(distance <= detectionRadius){
        bool isLineOfSightBlocked = false;

        for(const auto& wall : walls){
            if(CheckLineRectCollision(enemyCenter, playerCenter, wall)){
                isLineOfSightBlocked = true;
                break;
            }
        }

        if(!isLineOfSightBlocked){
            if(distance <= attackRadius){
                currentState = EnemyState::ATTACK;
            }else{
                currentState = EnemyState::CHASE;
            }
        }else{
            if(currentState == EnemyState::CHASE || currentState == EnemyState::ATTACK){
               currentState = EnemyState::RETURN;
            }
        }
    }else{
        if(currentState == EnemyState::CHASE || currentState == EnemyState::ATTACK){
            currentState = EnemyState::RETURN;
        }
    }
}

void Enemy::DoPatrolAction(float dt, const std::vector<Rectangle>& walls){
    actionTimer += dt;
    
    float moveX = 0.0f;
    float moveY = 0.0f;

    if(currentPatrolType == PatrolType::HORIZONTAL){
        moveX = (speed * moveDirection) * dt;
        MoveWithCollision(bounds, moveX, 0, walls);

        float distanceFromSpawn = bounds.x - spawnPoint.x;
        if(distanceFromSpawn > patrolRange){
            moveDirection = -1;
        }else if(distanceFromSpawn < -patrolRange){
            moveDirection = 1;
        }
    }else if(currentPatrolType == PatrolType::VERTICAL){
        moveY = (speed * moveDirection) * dt;
        MoveWithCollision(bounds, 0, moveY, walls);

        float distanceFromSpawn = bounds.y - spawnPoint.y;
        if(distanceFromSpawn > patrolRange){
            moveDirection = -1;
        }else if(distanceFromSpawn < -patrolRange){
            moveDirection = 1;
        }
    }

    if(actionTimer >= actionDuration){
        actionTimer = 0.0f;
        actionDuration = GetRandomValue(2, 4);
        currentAction = NormalAction::IDLE;
    }
}

void Enemy::ChasePlayer(Player &player, float dt, const std::vector<Rectangle>& walls){
    Vector2 enemyCenter = { bounds.x + bounds.width / 2, bounds.y + bounds.height/2};
    
    Rectangle playerBounds = player.GetBounds();
    Vector2 playerCenter = { playerBounds.x + playerBounds.width/2, playerBounds.y + playerBounds.height/2};

    Vector2 direction = { playerCenter.x - enemyCenter.x, playerCenter.y - enemyCenter.y};
    direction = Vector2Normalize(direction); // Unit Vector

    float moveX = direction.x * chase_speed * dt;
    float moveY = direction.y * chase_speed * dt;

    MoveWithCollision(bounds, moveX, moveY, walls);
}

void Enemy::DoIdleAction(float dt){
    actionTimer += dt;

    if((int)actionTimer % 2 == 0){}

    if(actionTimer >= actionDuration){
        actionTimer = 0.0f;
        actionDuration = GetRandomValue(3, 5);
        
        currentAction = NormalAction::PATROL;
    }
}

void Enemy::ReturnToSpawn(float dt, const std::vector<Rectangle>& walls){
    Vector2 enemyCenter = { bounds.x + bounds.width/2, bounds.y + bounds.height/2 };

    float distanceToSpawn = Vector2Distance(enemyCenter, spawnPoint);

    if(distanceToSpawn > 5.0f){
        Vector2 direction = { spawnPoint.x - enemyCenter.x, spawnPoint.y - enemyCenter.y };
        direction = Vector2Normalize(direction);

        float moveX = direction.x * speed * dt;
        float moveY = direction.y * speed * dt;

        MoveWithCollision(bounds, moveX, moveY, walls);
    }
    else{
        actionTimer = 0.0f;
        actionDuration = GetRandomValue(2, 4);
        currentAction = NormalAction::IDLE; // ตั้งต้นที่ยืนหมุนตัวก่อน
        moveDirection = 1;                  // รีเซ็ตทิศทางเริ่มเดินใหม่
        currentState = EnemyState::NORMAL;  // กลับสู่สถานะปกติสำเร็จ!
    }

    TraceLog(LOG_INFO, "Enemy Return.");
}

void Enemy::TakeCounterAttack(float damage, Vector2 attackDirection){
    health -= damage;

    if(health < 0.0f){
        health = 0.0f;
    }

    if(!IsDead()){
        bounds.x += attackDirection.x * knockbackForce;
        bounds.y += attackDirection.y * knockbackForce;

        isStunned = true;
        stunTimer = 0.75f;
    }

    TraceLog(LOG_INFO, "Enemy hit by Counter Attack!");
}