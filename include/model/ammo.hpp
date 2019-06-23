#ifndef AMMO_H
#define AMMO_H

#include "model/ammo_effect.hpp"
#include "model/base.hpp"
#include "model/enemy.hpp"

class Ammo : public GameObject
{
public:
    Ammo(std::string name,
         Enemy *target,
         float speed,
         float damage,
         AmmoEffects::AmmoEffect effect = AmmoEffects::NoneEffect(),
         glm::vec4 position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
         glm::vec4 orientation = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f),
         float angle = 0,
         glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f)) :
        GameObject(name, position, orientation, angle, scale), target(target), speed(speed), damage(damage), ammo_effect(effect), origin_position(position)
    {

    };

    virtual void update(float delta_time) = 0;

    // Verifies if it already finished it movement and can hit the target
    bool completed_movement()
    {
        return this->time >= 1.0f;
    }

protected:
    Enemy *target;
    float speed = 1.0f;                                                 // Number that we multiply with the delta_time to add to time
    float damage = 1.0f;                                                // Damage that this ammo does when hitting an enemy
    float time = 0.0f;                                                  // When this get to 1.0f, the ammo hits the target
    AmmoEffects::AmmoEffect ammo_effect = AmmoEffects::NoneEffect();    // Effect applied on hit or over time
    glm::vec4 origin_position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);      // Original ammo position to compute bézier curves


    // Hit the target
    void hit_enemy()
    {
        if (target != nullptr)
            target->hit(damage, &ammo_effect);
    }
};

#endif // AMMO_H
