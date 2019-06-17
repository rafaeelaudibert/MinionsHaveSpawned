#include "model/enemy.hpp"

class OrderSuper : public Enemy
{
public:
    OrderSuper(std::string name,
               EnemyColor color = EnemyColor::RED,
               glm::vec4 position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
               glm::vec4 orientation = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f),
               float angle = 0,
               glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f)) : Enemy(name, color, position, orientation, angle, scale)
    {
        this->build();
    };

    void build();
    void render(glm::mat4 view, glm::mat4 projection);
    void update(float delta_time, Camera *camera) override;
};
