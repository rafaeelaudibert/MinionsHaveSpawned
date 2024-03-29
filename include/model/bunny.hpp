#include "model/base.hpp"

// Bunny class, not actually used
class Bunny : public GameObject
{
public:
    Bunny(std::string name,
          glm::vec4 position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
          glm::vec4 orientation = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f),
          float angle = 0,
          glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f)) : GameObject(name, position, orientation, angle, scale)
    {
        this->build();
    };

    void build();

    void render(glm::mat4 view, glm::mat4 projection);
};
