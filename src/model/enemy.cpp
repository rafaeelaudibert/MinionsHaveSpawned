#include "model/enemy.hpp"
#include <algorithm>

void Enemy::update(float delta_time)
{
    for (AmmoEffects::AmmoEffect effect : this->effects)
    {
        effect.update(delta_time);
    }

    // Remove the effects which are over
    auto res = std::remove_if(this->effects.begin(), this->effects.end(), [](AmmoEffects::AmmoEffect const x)
    {
        return x.ammo_time <= 0.0f;
    });
    this->effects.erase(res, this->effects.end());

    float accumulated_damage = 0.0f;
    float accumulated_speed = 1.0f;
    for (const AmmoEffects::AmmoEffect effect : this->effects)
    {
        // Aoe effects are handled in a different way, splitting the damage to the enemies in a radius
        if (effect.ammo_type != "aoe") {
            accumulated_damage += effect.ammo_damage * delta_time;
            accumulated_speed *= effect.ammo_speed_multiplier;
        } else {
            // TODO: For enemies in a certain radial distance from this enemy, apply effect->ammo_damage once
        }
    }

    // Hit the damage accumulated in the computation above
    this->hit(accumulated_damage);

    // TODO: Walk multiplying by the speed computed above
}

bool Enemy::is_dead()
{
    return this->current_life_points < 0;
}

void Enemy::hit(float life_points, AmmoEffects::AmmoEffect *ammo_effect)
{
    if (!this->is_dead())
    {
        this->current_life_points -= life_points;

        if (ammo_effect != nullptr)
            this->effects.emplace_back(*ammo_effect);
    }

    return;
}

void Enemy::render_health_bar(glm::mat4 view, glm::mat4 projection)
{

    if (!this->is_dead())
        this->health_bar->render(view, projection, this->current_life_points / this->max_life_points);

    return;
}
