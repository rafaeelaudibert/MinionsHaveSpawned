#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <map>
#include <string>
#include <vector>

#include <glad/glad.h>
#include <tiny_obj_loader.h>

#include "engine/texture.hpp"
#include "engine/cubemap_texture.hpp"
#include "engine/shader.hpp"
#include "model/base.hpp"
#include "engine/matrices.hpp"

// A basic struct to hold the stuff we need to remember about loaded models
struct BasicModelStorage
{
    GLuint VAO;
    GLenum drawMode;
    unsigned int indexesLength;
    GLvoid* indexesOffset;
    glm::vec3 bbox_min;
    glm::vec3 bbox_max;
};

// A static singleton ResourceManager class that hosts several
// functions to load Textures and Shaders. Each loaded texture
// and/or shader is also stored for future reference by string
// handles. All functions and resources are static and no
// public constructor is defined.
class ResourceManager
{
public:
    // Resource storage
    static std::map<std::string, Shader> shaders;
    static std::map<std::string, Texture2D> textures;
    static std::map<std::string, CubemapTexture2D> cubemapTextures;
    static std::map<std::string, BasicModelStorage> models;

    // Loads (and generates) a shader program from file loading vertex, fragment (and geometry) shader's source code. If gShaderFile is not nullptr, it also loads a geometry shader
    static Shader load_shader(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile, std::string name);

    // Retrieves a stored sader
    static Shader get_shader(std::string name);

    // Loads (and generates) a texture from file
    static Texture2D load_texture(const GLchar *file, std::string name, GLboolean alpha = false);

    // Retrieves a stored texture
    static Texture2D get_texture(std::string name);

    // Loads a cubemap texture from 6 individual texture faces
    // order:
    // +X (right)
    // -X (left)
    // +Y (top)
    // -Y (bottom)
    // +Z (front)
    // -Z (back)
    static CubemapTexture2D load_cubemap_texture(std::vector<std::string> files, std::string name);

    // Retrieves a stored cubemap texture
    static CubemapTexture2D get_cubemap_texture(std::string name);

    // Loads an object from a .obj file and configure it
    static void load_object(const char *filename, GameObject *gameObject, std::string name);

    // Properly de-allocates all loaded resources
    static void clear();

private:
    // Private constructor, that is we do not want any actual resource manager objects. Its members and functions should be publicly available (static).
    ResourceManager() {}

    // Loads and generates a shader from file
    static Shader load_shader_from_file(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile = nullptr);

    // Loads a single texture from file
    static Texture2D load_texture_from_file(const GLchar *file, GLboolean alpha = false);

    // Loads 6 textures from a file, in a cubemapTexture2d form
    static CubemapTexture2D load_cubemap_texture_from_files(std::vector<std::string> files);

    // Compute the normals for a ObjModel object, read from a .obj file
    static void compute_normals(ObjModel *model);
};

#endif
