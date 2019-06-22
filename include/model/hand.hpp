#include "model/base.hpp"
#include "model/turret.hpp"

#ifndef HAND_H
#define HAND_H
class Hand : public GameObject
{
public:
    Hand(std::string name,
         glm::vec4 position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
         glm::vec4 orientation = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f),
         float angle = 0,
         glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f)) :
        GameObject(name, position, orientation, angle, scale)
    {
        this->build();
        this->turret = nullptr;
    };

    void build();

    void render(glm::mat4 view, glm::mat4 projection);
    void render_turret(glm::mat4 view, glm::mat4 projection);

    // The hand might me holding a turret
    Turret *turret;
};
#endif // HAND_H

