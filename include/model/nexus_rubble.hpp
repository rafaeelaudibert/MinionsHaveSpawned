#include "model/collisive.hpp"
#include "engine/camera.hpp"
#include "model/health_bar.hpp"

#ifndef NEXUS_RUBBLE_H
#define NEXUS_RUBBLE_H
enum class NexusSide
{
    CHAOS,
    ORDER
};

class NexusRubble : public Collisive
{
public:
    NexusRubble(std::string name,
                NexusSide side,
                glm::vec4 position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
                glm::vec4 orientation = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f),
                float angle = 0,
                glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f)) : Collisive(name, position, orientation, angle, scale), side(side)
    {
        this->build();
    };

protected:
    NexusSide side = NexusSide::CHAOS;


public:

    // Build and render
    void build();
    void render(glm::mat4 view, glm::mat4 projection);
};

#endif // NEXUS_H
