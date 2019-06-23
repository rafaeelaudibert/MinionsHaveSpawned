#include "model/collisive.hpp"
#include "engine/camera.hpp"
#include "model/health_bar.hpp"
#include "model/ammo_effect.hpp"
#include <vector>

#ifndef ENEMY_H
#define ENEMY_H

enum class EnemyColor {
    RED,
    BLUE
};

enum class WalkingPhase {
    FIRST,
    SECOND
};

enum class Route {
    TOP,
    MID,
    BOTTOM
};


class Enemy : public Collisive
{
protected:
    Enemy(std::string name,
          EnemyColor color,
          Route route = Route::MID,
          glm::vec4 position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
          glm::vec4 orientation = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f),
          float angle = 0,
          glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f)) : Collisive(name, position, orientation, angle, scale), color(color)
    { };

public:
    EnemyColor color = EnemyColor::RED;

    //Life properties
    float current_life_points = 0;
    float max_life_points = 0;
    HealthBar *health_bar;
    std::vector<AmmoEffects::AmmoEffect> effects;


    // Movement properties
    float speed = 1 / 40.0f;                            // 1 / seconds needed to go through half lane (or the mid lane)
    Route route = Route::MID;
    WalkingPhase walking_phase = WalkingPhase::FIRST;
    float walking_phase_step = 0.0f;


public:

    // Creating virtual desconstructor
    virtual ~Enemy() { };

    // Update function (moves the object, and checks the collision with the camera to move it
    virtual void update(float);

    // Returns the model matrix for the object
    glm::mat4 model_matrix();

    // Returns if the enemy is dead
    bool is_dead();

    // Give damage to the enemy
    void hit(float life_points, AmmoEffects::AmmoEffect *ammo_effect = nullptr);

    // Render health_bar
    void render_health_bar(glm::mat4, glm::mat4);
};

#endif // ENEMY_H
