#include "model/turret.hpp"

class HowlingChaos : public Turret
{
public:
    HowlingChaos(std::string name,
                 TurretColor color,
                 float recharge_time = 2.0f,
                 glm::vec4 position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
                 glm::vec4 orientation = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f),
                 float angle = 0,
                 glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f)) : Turret(name, color, recharge_time, position, orientation, angle, scale)
    {
        this->build();
    };


    void build();
    void render(glm::mat4 view, glm::mat4 projection);
};