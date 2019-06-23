#include "model/collisive.hpp"
#include "model/enemy.hpp"

#include <map>
#include <string>

#ifndef TURRET_H
#define TURRET_H
enum class TurretColor{
    RED,
    BLUE
};

class Turret : public Collisive
{
protected:
    Turret(std::string name,
          TurretColor color,
          bool placed = false,
          float recharge_time = 2.0f,
          glm::vec4 position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
          glm::vec4 orientation = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f),
          float angle = 0,
          glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f)) : Collisive(name, position, orientation, angle, scale), color(color), placed(placed)
    {

    };

public:
    virtual ~Turret() {}; // Virtual destructor

    TurretColor color = TurretColor::RED;

    // Shooting
    glm::vec4 shooting_position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    float recharge_time = 2.0f;     // Time to recharge
    float elapsed_time = 0.0f;      // Elapsed time since the last shot
    Enemy *target;                  // Current target

    // Placing
    bool placed = false;

    // Update function (updates all its ammo and change current target)
    virtual void update(float delta_time) = 0;
    virtual bool can_place();

private:
    bool out_of_map();
};

#endif // TURRET_H
