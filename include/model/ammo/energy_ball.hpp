#include "model/ammo.hpp"

#ifndef ENERGY_BALL_H
#define ENERGY_BALL_H

// Ammo used by the SummonersChaos, SummonersOrders, HowlingChaos e HowlingOrder turrets, with different AmmoEffects
class EnergyBall : public Ammo
{
public:
    EnergyBall(std::string name,
               Enemy *target,
               float speed,
               float damage,
               AmmoEffects::AmmoEffect effect = AmmoEffects::NoneEffect(),
               glm::vec4 position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
               glm::vec4 orientation = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f),
               float angle = 0,
               glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f)) :
        Ammo(name, target, speed, damage, effect, position, orientation, angle, scale)
    {
        this->build();
    };

    ~EnergyBall() { };

    // Declare virtual functions
    void build();
    void update(float delta_time);
    void render(glm::mat4 view, glm::mat4 projection);
};

#endif // ENERGY_BALL_H
