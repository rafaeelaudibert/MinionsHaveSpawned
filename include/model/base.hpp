#ifndef GAME_OBJECT_H_
#define GAME_OBJECT_H_

#include <string>
#include <glad/glad.h>
#include <tiny_obj_loader.h>
#include "engine/shader.hpp"
#include "engine/texture.hpp"
#include "model/obj_model.hpp"

class GameObject
{
protected:
    GameObject(std::string name,
               glm::vec4 position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
               glm::vec4 orientation = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f),
               float angle = 0,
               glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f)) : name(name), position(position), orientation(orientation), angle(angle), scale(scale){};

public:
    virtual void render(glm::mat4 view, glm::mat4 projection) = 0;

    std::string name;

private:
    virtual void build() = 0;

    // Public for now, should be protected
public:
    GLuint VAO;
    Shader shader;
    Texture2D texture;

    GLenum drawMode;
    GLsizei indexesLength;
    GLvoid *indexesOffset;
    glm::vec4 position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    glm::vec4 orientation = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
    float angle = 0;
    glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);

    glm::vec3 bbox_min;
    glm::vec3 bbox_max;
};

#endif // GAME_OBJECT_H_
