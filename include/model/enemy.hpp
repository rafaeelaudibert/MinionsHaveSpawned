#include "model/collisive.hpp"
#include "engine/camera.hpp"
#include "model/health_bar.hpp"
#include "model/ammo_effect.hpp"
#include <vector>

#ifndef ENEMY_H
#define ENEMY_H

// Possible colors for an enemy to take
enum class EnemyColor {
    RED,
    BLUE
};

// Which part of the path to the enemy base the enemy is
enum class WalkingPhase {
    FIRST,
    SECOND
};

// Which route is the enemy going to go through, currently only implemented MID, with the other fallbacking to mid
enum class Route {
    TOP,
    MID,
    BOTTOM
};

// Enemy class, which all the enemies should inherit from, as they contain important information required for the
// game mechanics
class Enemy : public Collisive
{
protected:
    Enemy(std::string name,
          EnemyColor color,
          Route route = Route::MID,
          float gold_reward = 0.0f,
          glm::vec4 position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
          glm::vec4 orientation = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f),
          float angle = 0,
          glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f)) : Collisive(name, position, orientation, angle, scale), color(color), route(route), gold_reward(gold_reward)
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

    // Gold properties
    float gold_reward = 0.0f;


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
