#include "model/enemy.hpp"

void Enemy::update(float delta_time, Camera *camera) {
    const float multiplier = 0.5;
    const float delta_position = multiplier * delta_time;

    this->position.z += delta_position;

    if (camera->check_collision(this)) {
        camera->position.z += delta_position;
    }
}
