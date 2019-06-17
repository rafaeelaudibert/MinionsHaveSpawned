#include "model/collisive.hpp"
#include "engine/camera.hpp"

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

    //Life points properties
    unsigned int max_life_points = 0;
    unsigned int current_life_points = 0;

    // Movement properties
    float bezier_time = 0.0f;

    // Update function (moves the object, and checks the collision with the camera to move it
    virtual void update(float delta_time, Camera *camera);
};

#endif // ENEMY_H
