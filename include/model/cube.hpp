#include "model\base.hpp"

class Cube : public GameObject
{
public:
    Cube(std::string name,
         glm::vec4 position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
         glm::vec4 orientation = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f),
         float angle = 0,
         glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f),
         const GLchar* texture_path = NULL,
         const GLchar* texture_name = NULL) :
        GameObject(name, position, orientation, angle, scale)
    {
        if (texture_path != NULL && texture_name != NULL)
            this->build(texture_path, texture_name);
        else
            this->build();
    };

    void build();
    void build(const GLchar* texture_path, const GLchar* texture_name);

    void render(glm::mat4 view, glm::mat4 projection);
};
