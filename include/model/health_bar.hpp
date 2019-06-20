#include "model/base.hpp"

#ifndef HEALTH_BAR_H
#define HEALTH_BAR_H

class HealthBar : public GameObject
{
protected:
    glm::vec4 color = glm::vec4(0.03f, 0.88f, 0.29f, 1.0f);

public:
    HealthBar(std::string name,
              glm::vec4 position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
              glm::vec4 orientation = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f),
              float angle = 0,
              glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f)) : GameObject(name, position, orientation, angle, scale)
    {
        this->build();
    };

    void build();

    void render(glm::mat4, glm::mat4);
    void render(glm::mat4, glm::mat4, float);
};
#endif // HEALTH_BAR_H


