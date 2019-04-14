#ifndef GAME_OBJECT_H_
#define GAME_OBJECT_H_

#include <string>
#include <glad/glad.h>

class GameObject
{
public:
    std::string name;
    GLuint VAO;

    GameObject(std::string name, GLuint VAO) : name(name), VAO(VAO) { };
};

#endif // GAME_OBJECT_H_

