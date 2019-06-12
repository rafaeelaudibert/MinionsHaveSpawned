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

    Cube(std::string name, glm::vec4 position, glm::vec4 orientation, float angle, glm::vec3 scale) :
        GameObject(name, position, orientation, angle, scale)
    {
        this->build();
    };

    Cube(std::string name, glm::vec4 position, glm::vec4 orientation, float angle, glm::vec3 scale, const GLchar* texture_path, const GLchar* texture_name) :
        GameObject(name, position, orientation, angle, scale)
    {
        this->build(texture_path, texture_name);
    };

    void build();
    void build(const GLchar* texture_path, const GLchar* texture_name);

    void render(glm::mat4 view, glm::mat4 projection);
};
