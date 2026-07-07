#include "game_object.h"

GameObject::GameObject(float x, float y, float width, float height)
    :bounds{x, y, width, height}, isActive(true) {}

GameObject::~GameObject(){
    TraceLog(LOG_INFO, "GameObject Destroyed");
}