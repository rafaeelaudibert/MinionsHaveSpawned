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

        // Configure top position and bot position
        const glm::vec4 top_position = glm::vec4(Constants::CHAOS_NEXUS_POSITION.x, Constants::CHAOS_NEXUS_POSITION.y, Constants::ORDER_NEXUS_POSITION.z, 1.0f);
        const glm::vec4 bottom_position = glm::vec4(Constants::ORDER_NEXUS_POSITION.x, Constants::ORDER_NEXUS_POSITION.y, Constants::CHAOS_NEXUS_POSITION.z, 1.0f);

        // Choose route to walk to
        glm::vec4 new_position;
        switch(this->route)
        {
        case Route::TOP:
            this->walking_phase_step += delta_time * accumulated_speed * this->speed * 10.0f;
            if (this->walking_phase_step >= 1.0f && this->walking_phase == WalkingPhase::FIRST) // Need to go to the other walking_phase
            {
                this->walking_phase = WalkingPhase::SECOND;
                this->walking_phase_step -= 1.0f;
            }

            // In TOP, we need to take care of the walking_phase
            if (this->walking_phase == WalkingPhase::FIRST)
            {
                new_position = Constants::CHAOS_NEXUS_POSITION + (top_position - Constants::CHAOS_NEXUS_POSITION) * this->walking_phase_step;
                this->angle = std::sin(this->walking_phase_step * 3.14159f * Constants::MINION_GIGGLES) * 0.2f;
            }
            else
            {
                new_position = top_position + (Constants::ORDER_NEXUS_POSITION - top_position) * this->walking_phase_step;
                this->angle = -3.14159f / 2.0f + std::sin(this->walking_phase_step * 3.14159f * Constants::MINION_GIGGLES) * 0.2f;
            }

            break;
        case Route::BOTTOM:
            this->walking_phase_step += delta_time * accumulated_speed * this->speed;
            if (this->walking_phase_step >= 1.0f && this->walking_phase == WalkingPhase::FIRST) // Need to go to the other walking_phase
            {
                this->walking_phase = WalkingPhase::SECOND;
                this->walking_phase_step -= 1.0f;
            }

            // In BOTTOM, we need to take care of the walking_phase
            if (this->walking_phase == WalkingPhase::FIRST)
            {
                new_position = Constants::CHAOS_NEXUS_POSITION + (bottom_position - Constants::CHAOS_NEXUS_POSITION) * this->walking_phase_step;
                this->angle = -3.14159f / 2.0f + std::sin(this->walking_phase_step * 3.14159f * Constants::MINION_GIGGLES) * 0.2f;
            }
            else
            {
                new_position = bottom_position + (Constants::ORDER_NEXUS_POSITION - bottom_position) * this->walking_phase_step;
                this->angle = std::sin(this->walking_phase_step * 3.14159f * Constants::MINION_GIGGLES) * 0.2f;
            }

            break;
        case Route::MID:
            // In MID doesn't matter the walking phase, we only have one
            this->walking_phase_step += delta_time * accumulated_speed * this->speed;
            new_position = Constants::CHAOS_NEXUS_POSITION + (Constants::ORDER_NEXUS_POSITION - Constants::CHAOS_NEXUS_POSITION) * this->walking_phase_step;
            this->angle = -3.14159f / 4.0f + std::sin(this->walking_phase_step * 3.14159f * Constants::MINION_GIGGLES) * 0.2f; // 80 small giggles

            break;
        }

        glm::vec4 movement = new_position - this->position;
        this->health_bar->position += movement;
        this->position += movement;

        if (Game::camera.check_collision(this))
        {
            Game::camera.position += movement;
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
