#include "model/enemy.hpp"

void Enemy::update(float delta_time, Camera *camera) {
    if (!this->is_dead()) {
        const float multiplier = 0.5;
        const float delta_position = multiplier * delta_time;

        // Update its position, and the healthbar
        this->position.z += delta_position;
        this->health_bar->position.z += delta_position;

        if (camera->check_collision(this)) {
            camera->position.z += delta_position;
        }
    }
}

bool Enemy::is_dead(){
    return this->current_life_points < 0;
}

void Enemy::hit(float life_points) {
    if (!this->is_dead())
        this->current_life_points -= life_points;
}
