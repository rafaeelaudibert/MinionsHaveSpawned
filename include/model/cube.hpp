#include "model\base.hpp"

class Cube : public GameObject
{
public:
    Cube(std::string name) : GameObject(name)
    {
        this->build();
    };

    Cube(std::string name, glm::vec4 position) : GameObject(name, position)
    {
        this->build();
    };

    Cube(std::string name, glm::vec4 position, glm::vec4 orientation, float angle) :
        GameObject(name, position, orientation, angle)
    {
        this->build();
    };

    void build();

    void render(glm::mat4 view, glm::mat4 projection);
};
