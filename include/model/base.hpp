#ifndef GAME_OBJECT_H_
#define GAME_OBJECT_H_

#include <string>
#include <glad/glad.h>
#include "engine/shader.hpp"

class GameObject
{
public:
    std::string name;
    GLuint VAO;
    Shader shader;

    GameObject(std::string name) : name(name) { };

    virtual void render(glm::mat4 view, glm::mat4 projection, int provisory) = 0;

protected:
    GLenum drawMode;
    GLsizei indexesLength;
    GLvoid *indexesOffset;
};

#endif // GAME_OBJECT_H_

