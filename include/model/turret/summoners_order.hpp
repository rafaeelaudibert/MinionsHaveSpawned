#include "model/turret.hpp"

class SummonersOrder : public Turret
{
public:
    SummonersOrder(std::string name,
                   TurretColor color,
                   bool placed = false,
                   float recharge_time = 2.0f,
                   glm::vec4 position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
                   glm::vec4 orientation = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f),
                   float angle = 0,
                   glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f)) : Turret(name, color, placed, recharge_time, position, orientation, angle, scale)
    {
        this->build();
    };

    ~SummonersOrder() {};


    void build();
    void render(glm::mat4 view, glm::mat4 projection);
    void update(float delta_time);
};
