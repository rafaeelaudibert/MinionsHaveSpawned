#include "model/base.hpp"

class Dummy : public GameObject
{
public:
    Dummy(std::string name) : GameObject(name)
    {
        this->build();
    };

    Dummy(std::string name, glm::vec4 position) : GameObject(name, position)
    {
        this->build();
    };

    Dummy(std::string name, glm::vec4 position, glm::vec4 orientation, float angle) :
        GameObject(name, position, orientation, angle)
    {
        this->build();
    };

    Dummy(std::string name, glm::vec4 position, glm::vec4 orientation, float angle, glm::vec3 scale) :
        GameObject(name, position, orientation, angle, scale)
    {
        this->build();
    };

    void build();

    void render(glm::mat4 view, glm::mat4 projection);

    GLuint texture_id;
};
