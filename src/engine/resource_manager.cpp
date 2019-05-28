#include "engine/resource_manager.hpp"

#include <iostream>
#include <sstream>
#include <fstream>

// Texture library
#include <stb_image.h>

// Instantiate static variables
std::map<std::string, Texture2D> ResourceManager::textures;
std::map<std::string, CubemapTexture2D> ResourceManager::cubemapTextures;
std::map<std::string, Shader> ResourceManager::shaders;

Shader ResourceManager::load_shader(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile, std::string name)
{
    shaders[name] = load_shader_from_file(vShaderFile, fShaderFile, gShaderFile);
    return shaders[name];
}

Shader ResourceManager::get_shader(std::string name)
{
    return shaders[name];
}



Texture2D ResourceManager::load_texture(const GLchar *file, GLboolean alpha, std::string name)
{
    textures[name] = load_texture_from_file(file, alpha);
    return textures[name];
}

Texture2D ResourceManager::get_texture(std::string name)
{
    return textures[name];
}

CubemapTexture2D ResourceManager::load_cubemap_texture(std::vector<std::string> files, std::string name)
{
    cubemapTextures[name] = load_cubemap_texture_from_files(files);
    return cubemapTextures[name];
}

CubemapTexture2D ResourceManager::get_cubemap_texture(std::string name)
{
    return cubemapTextures[name];
}

Shader ResourceManager::load_shader_from_file(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile)
{
    // 1. Retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;

    try
    {
        // Open files
        std::ifstream vertexShaderFile(vShaderFile);
        std::ifstream fragmentShaderFile(fShaderFile);
        std::stringstream vShaderStream, fShaderStream;

        // Read file's buffer contents into streams
        vShaderStream << vertexShaderFile.rdbuf();
        fShaderStream << fragmentShaderFile.rdbuf();

        // close file handlers
        vertexShaderFile.close();
        fragmentShaderFile.close();

        // Convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();

        // If geometry shader path is present, also load a geometry shader
        if (gShaderFile != nullptr)
        {
            std::ifstream geometryShaderFile(gShaderFile);
            std::stringstream gShaderStream;
            gShaderStream << geometryShaderFile.rdbuf();
            geometryShaderFile.close();
            geometryCode = gShaderStream.str();
        }
    }
    catch (std::exception e)
    {
        std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
    }

    const GLchar *vShaderCode = vertexCode.c_str();
    const GLchar *fShaderCode = fragmentCode.c_str();
    const GLchar *gShaderCode = geometryCode.c_str();

    // 2. Now create shader object from source code
    Shader shader;
    shader.compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
    return shader;
}

Texture2D ResourceManager::load_texture_from_file(const GLchar *filename, GLboolean alpha)
{
    // Create Texture object
    Texture2D texture;

    printf("Loading image \"%s\"... ", filename);

    if (alpha)
    {
        texture.internal_format = GL_RGBA;
        texture.image_format = GL_RGBA;
    }

    // Primeiro fazemos a leitura da imagem do disco
    stbi_set_flip_vertically_on_load(true);
    int width;
    int height;
    int channels;
    unsigned char *image = stbi_load(filename, &width, &height, &channels, 0);

    if ( image == NULL )
    {
        fprintf(stderr, "ERROR: Cannot open image file \"%s\".\n", filename);
        std::exit(EXIT_FAILURE);
    }

    printf("OK (%dx%d).\n", width, height);

    // Now generate texture
    texture.generate(width, height, image);

    // And finally free image
    stbi_image_free(image);

    return texture;
}

CubemapTexture2D ResourceManager::load_cubemap_texture_from_files(std::vector<std::string> faces)
{
    CubemapTexture2D texture;

    texture.generate(faces);

    return texture;
}

void ResourceManager::clear()
{
    // (Properly) delete all shaders
    for (auto iter : shaders)
        glDeleteProgram(iter.second.ID);

    // (Properly) delete all textures
    for (auto iter : textures)
        glDeleteTextures(1, &iter.second.ID);
}
