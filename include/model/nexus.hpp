#include "model/collisive.hpp"
#include "engine/camera.hpp"
#include "model/health_bar.hpp"
#include "model/nexus_rubble.hpp"

#ifndef NEXUS_H
#define NEXUS_H
enum class NexusColorSide
{
    CHAOS_RED,
    CHAOS_BLUE,
    ORDER_RED,
    ORDER_BLUE
};

class Nexus : public Collisive
{
public:
    Nexus(std::string name,
          NexusColorSide color,
          glm::vec4 position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
          glm::vec4 orientation = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f),
          float angle = 0,
          glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f)) : Collisive(name, position, orientation, angle, scale), color(color)
    {
        this->build();
        this->health_bar = new HealthBar(this->name + "_healthbar", glm::vec4(this->position.x, this->bbox_max.y * scale.y + 0.5f, this->position.z, 1.0f), glm::vec4(0.0f, 1.0f, 0.0f, 0.0f), 0, glm::vec3(glm::vec4(0.5f, 0.4f, 1.0f, 1.0f) * matrix::scale_matrix(this->scale)));
        this->max_life_points = this->current_life_points = 3000;

        this->nexus_rubble = new NexusRubble(this->name + "_rubble", (this->color == NexusColorSide::CHAOS_RED || this->color == NexusColorSide::CHAOS_BLUE) ? NexusSide::CHAOS : NexusSide::ORDER, this->position, this->orientation, this->angle, this->scale);
    };

    NexusColorSide color = NexusColorSide::CHAOS_RED;

    //Life properties
    float current_life_points = 0;
    float max_life_points = 0;
    HealthBar *health_bar;

    // NexusRubble counter part
    NexusRubble *nexus_rubble;


public:

    // Returns if the nexus is dead
    bool is_dead();

    // Give damage to the nexus
    void hit(float);

    // Render health_bar
    void render_health_bar(glm::mat4, glm::mat4);

    // Build and render
    void build();
    void render(glm::mat4 view, glm::mat4 projection);
};

#endif // NEXUS_H
