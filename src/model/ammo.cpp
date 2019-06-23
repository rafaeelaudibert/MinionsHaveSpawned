#include "model/ammo.hpp"

float Ammo::AOE_RADIUS = 3.5f;

// Returns true if the ammo has already reached the enemy
bool Ammo::completed_movement()
{
    return this->time >= 1.0f;
}

// Checks the ability to hit the enemy
void Ammo::hit_enemy()
{
    if (this->target != nullptr)
        this->target->hit(this->damage, &this->ammo_effect);
}
