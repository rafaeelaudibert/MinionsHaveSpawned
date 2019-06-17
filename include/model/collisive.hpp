#include "model/base.hpp"

#ifndef COLLISIVE_H
#define COLLISIVE_H

class Collisive : public GameObject
{
protected:
    Collisive(std::string name,
              glm::vec4 position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
              glm::vec4 orientation = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f),
              float angle = 0,
              glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f)) : GameObject(name, position, orientation, angle, scale)
    {

    };

public:
    bool collide(glm::vec3 bbox_min, glm::vec3 bbox_max);
    bool collide(Collisive *collisive);
};

#endif // COLLISIVE_H
