#include "model/collisive.hpp"

#ifndef ENEMY_H
#define ENEMY_H
enum EnemyColor {
    RED,
    BLUE
};

class Enemy : public Collisive
{
protected:
    Enemy(std::string name,
          EnemyColor color,
          glm::vec4 position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
          glm::vec4 orientation = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f),
          float angle = 0,
          glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f)) : Collisive(name, position, orientation, angle, scale), color(color)
    {

    };

public:
    EnemyColor color = EnemyColor::RED;
};

#endif // ENEMY_H
