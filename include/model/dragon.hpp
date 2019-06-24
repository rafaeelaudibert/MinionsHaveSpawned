#include "model/base.hpp"

// Possible dragon types
enum DragonType
{
    AIR,
    EARTH,
    FIRE,
    WATER,
    ELDER
};

// Basic dragon model, not in use currently
class Dragon : public GameObject
{
public:
    Dragon(std::string name,
           glm::vec4 position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
           glm::vec4 orientation = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f),
           float angle = 0,
           glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f),
           DragonType type = DragonType::ELDER) :
        GameObject(name, position, orientation, angle, scale)
    {
        this->type = type;
        this->build();
    };

    void build();

    void render(glm::mat4 view, glm::mat4 projection);

    DragonType type = DragonType::ELDER;
};
