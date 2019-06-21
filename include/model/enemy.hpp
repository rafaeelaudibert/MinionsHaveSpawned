#include "model/collisive.hpp"
#include "engine/camera.hpp"
#include "model/health_bar.hpp"

#ifndef ENEMY_H
#define ENEMY_H
enum class EnemyColor {
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
    { };

    EnemyColor color = EnemyColor::RED;

    //Life properties
    float current_life_points = 0;
    float max_life_points = 0;
    HealthBar *health_bar;

    // Movement properties
    float bezier_time = 0.0f;


public:

    // Update function (moves the object, and checks the collision with the camera to move it
    virtual void update(float, Camera *);

    // Returns if the enemy is dead
    bool is_dead();

    // Give damage to the enemy
    void hit(float);

    // Render health_bar
    void render_health_bar(glm::mat4, glm::mat4);
};

#endif // ENEMY_H
