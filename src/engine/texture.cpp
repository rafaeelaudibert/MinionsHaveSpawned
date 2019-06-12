#include <iostream>

#include "engine/texture.hpp"
#include "engine/resource_manager.hpp"

Texture2D::Texture2D()
    : width(0), height(0), wrap_s(GL_REPEAT), wrap_t(GL_REPEAT), filter_min(GL_LINEAR), filter_max(GL_LINEAR)
{ }


void Texture2D::generate(const GLchar* filename)
{
    // Generate textures
    glGenTextures(1, &this->ID);
    glBindTexture(GL_TEXTURE_2D, this->ID);

    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->wrap_s);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->wrap_t);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->filter_min);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->filter_max);

    // load and generate the texture
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    this->width = width;
    this->height = height;
}

void Texture2D::bind() const
{
    glBindTexture(GL_TEXTURE_2D, this->ID);
}
