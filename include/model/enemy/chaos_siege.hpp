#include "model/enemy.hpp"

#ifndef CHAOS_SIEGE_H
#define CHAOS_SIEGE_H

// Implementation of an enemy
class ChaosSiege : public Enemy
{
public:
    ChaosSiege(std::string name,
               EnemyColor color = EnemyColor::RED,
               glm::vec4 position = Constants::CHAOS_NEXUS_POSITION,
               glm::vec4 orientation = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f),
               float angle = 0,
               glm::vec3 scale = glm::vec3(1.5f, 1.5f, 1.5f)) : Enemy(name, color, Route::MID, 10.0f, position, orientation, angle, scale)
    {
        this->build();
        this->health_bar = new HealthBar(this->name + "_healthbar", glm::vec4(this->position.x, this->bbox_max.y * scale.y  + 0.5f, this->position.z, 1.0f), glm::vec4(0.0f, 1.0f, 0.0f, 0.0f), 0, glm::vec3(glm::vec4(0.5f, 0.4f, 1.0f, 1.0f) * matrix::scale_matrix(this->scale)));
        this->max_life_points = this->current_life_points = 150;
    };

    ~ChaosSiege() { };

    void build();
    void render(glm::mat4 view, glm::mat4 projection);
};
#endif // CHAOS_SIEGE_H

