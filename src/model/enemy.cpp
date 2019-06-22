#include "model/enemy.hpp"

void Enemy::update(float delta_time) {
    for (AmmoEffects::AmmoEffect effect : this->effects) {
        effect.update(delta_time);
    }

    // Remove the effects which are over
    auto res = std::remove_if(this->effects.begin(), this->effects.end(), [](auto const x) {return x.ammo_time <= 0.0f});
    this->effects.erase(res, this->effects.end());

    // TODO: Need to figure it out how to properly use the effects applied
}

bool Enemy::is_dead(){
    return this->current_life_points < 0;
}

void Enemy::hit(float life_points, AmmoEffects::AmmoEffect *ammo_effect) {
    if (!this->is_dead()) {
        this->current_life_points -= life_points;

        if (ammo_effect != nullptr)
            this->effects.emplace_back(*ammo_effect);
    }

    return;
}

void Enemy::render_health_bar(glm::mat4 view, glm::mat4 projection) {

    if (!this->is_dead())
        this->health_bar->render(view, projection, this->current_life_points / this->max_life_points);

    return;
}
