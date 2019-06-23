#include "model/enemy.hpp"

class OrderSiege : public Enemy
{
public:
    OrderSiege(std::string name,
               EnemyColor color = EnemyColor::RED,
               glm::vec4 position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
               glm::vec4 orientation = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f),
               float angle = 0,
               glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f)) : Enemy(name, color, position, orientation, angle, scale)
    {
        this->build();
        this->health_bar = new HealthBar(this->name + "_healthbar", glm::vec4(this->position.x, this->bbox_max.y * scale.y  + 0.5f, this->position.z, 1.0f), glm::vec4(0.0f, 1.0f, 0.0f, 0.0f), 0, glm::vec3(glm::vec4(0.5f, 0.4f, 1.0f, 1.0f) * matrix::scale_matrix(this->scale)));
        this->max_life_points = this->current_life_points = 100;
    };

    ~OrderSiege() { };

    void build();
    void render(glm::mat4 view, glm::mat4 projection);
};
