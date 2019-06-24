#include "model/enemy.hpp"
#include <algorithm>

void Enemy::update(float delta_time)
{
    if (!this->is_dead())
    {
        for (AmmoEffects::AmmoEffect& effect : this->effects)
        {
            effect.update(delta_time);
        }

        float accumulated_damage = 0.0f;
        float accumulated_speed = 1.0f;
        for (const AmmoEffects::AmmoEffect effect : this->effects)
        {
            // Aoe effects are handled in a different way, splitting the damage to the enemies in a radius
            if (effect.ammo_type != "aoe")
            {
                accumulated_damage += effect.ammo_damage * delta_time;
                accumulated_speed *= effect.ammo_speed_multiplier;
            }
            else
            {
                // For enemies in a certain radial distance from this enemy, apply effect.ammo_damage once
                for (const auto &object : Game::enemy_objects)
                {
                    float enemy_distance = matrix::norm(object.second->position - this->position);
                    if (enemy_distance <= Ammo::AOE_RADIUS && object.second != this)
                    {
                        object.second->hit(effect.ammo_damage / 2);
                    }
                }
            }
        }

        // Remove the effects which are over
        auto res = std::remove_if(this->effects.begin(), this->effects.end(), [](AmmoEffects::AmmoEffect const x)
        {
            return x.ammo_time <= 0.0f;
        });
        this->effects.erase(res, this->effects.end());

        // Hit the damage accumulated in the computation above
        this->hit(accumulated_damage);

        switch(this->route)
        {
        case Route::TOP:
        case Route::BOTTOM:
        case Route::MID:
            // In MID doesn't matter the walking phase, we only have one
            this->walking_phase_step += delta_time * accumulated_speed * this->speed;
            glm::vec4 new_position = Constants::CHAOS_NEXUS_POSITION + (Constants::ORDER_NEXUS_POSITION - Constants::CHAOS_NEXUS_POSITION) * this->walking_phase_step;
            this->angle = -3.14159f / 8.0f + std::sin(this->walking_phase_step * 3.14159f * 60.0f) * 0.2f; // 20 small giggles

            glm::vec4 movement = new_position - this->position;
            this->health_bar->position += movement;
            this->position += movement;

            if (Game::camera.check_collision(this))
            {
                Game::camera.position += movement;
            }
            break;
        }
    }
    else     // Slowly goes to beneath the ground
    {
        this->position.y -= Constants::MINION_DEATH_SPEED * delta_time;
    }

}

bool Enemy::is_dead()
{
    return this->current_life_points <= 0;
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

glm::mat4 Enemy::model_matrix()
{
    return matrix::translate_matrix(this->position) * matrix::scale_matrix(this->scale) * matrix::rotate_matrix(this->angle, this->orientation);
}
