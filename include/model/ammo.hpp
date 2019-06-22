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
         AmmoEffect effect = AmmoEffect::NoneEffect(),
         glm::vec4 position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
         glm::vec4 orientation = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f),
         float angle = 0,
         glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f)) :
        GameObject(name, position, orientation, angle, scale), target(target), speed(speed), damage(damage), ammo_effect(effect)
    {
        this->build();
    };

    virtual void update(float delta_time) = 0;

    void hit_enemy()
    {
        if (target != nullptr)
            target->hit(damage, ammo_effect);
    }

    bool should_hit_enemy()
    {
        return this->time >= 1.0f;
    }

private:
    Enemy *target;
    float speed = 1.0f;                                 // Number that we multiply with the delta_time to add to time
    float damage = 1.0f;                                // Damage that this ammo does when hitting an enemy
    float time = 0.0f;                                  // When this get to 1.0f, the ammo hits the target
    AmmoEffect ammo_effect = AmmoEffect::NoneEffect();  // Effect applied on hit or over time
};

#endif // AMMO_H
