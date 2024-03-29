#ifndef SHADER_H
#define SHADER_H

#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// General purpose shader object. Compiles from file, generates
// compile/link-time error messages and hosts several utility
// functions for easy management.
class Shader
{
public:
    // Variables
    GLuint ID;

    // Constructor
    Shader() : ID(glCreateProgram()) {}

    // Sets the current shader as active
    Shader &use();

    // Compiles the shader from given source code
    void compile(const GLchar *vertexSource, const GLchar *fragmentSource, const GLchar *geometrySource = nullptr);

    // Utility functions
    void set(const GLchar *name, GLfloat value, GLboolean useShader = false);
    void set(const GLchar *name, GLint value, GLboolean useShader = false);
    void set_vector(const GLchar *name, GLfloat x, GLfloat y, GLboolean useShader = false);
    void set_vector(const GLchar *name, const glm::vec2 &value, GLboolean useShader = false);
    void set_vector(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader = false);
    void set_vector(const GLchar *name, const glm::vec3 &value, GLboolean useShader = false);
    void set_vector(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean useShader = false);
    void set_vector(const GLchar *name, const glm::vec4 &value, GLboolean useShader = false);
    void set_matrix(const GLchar *name, const glm::mat4 &matrix, GLboolean useShader = false);

private:
    // Checks if compilation or linking failed and if so, print the error logs
    void check_compile_errors(GLuint object, std::string type);
};

#endif
