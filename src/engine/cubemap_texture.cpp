#include <iostream>
#include <vector>

#include "engine/cubemap_texture.hpp"

CubemapTexture2D::CubemapTexture2D()
    : width(0), height(0), internal_format(GL_RGB), image_format(GL_RGB), wrap_s(GL_CLAMP_TO_EDGE), wrap_t(GL_CLAMP_TO_EDGE), wrap_r(GL_CLAMP_TO_EDGE), filter_min(GL_LINEAR), filter_max(GL_LINEAR)
{
    glGenTextures(1, &this->ID);
}


void CubemapTexture2D::generate(std::vector<std::string> faces)
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->ID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, this->internal_format, width, height, 0, this->image_format, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, this->filter_min);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, this->filter_max);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, this->wrap_s);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, this->wrap_t);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, this->wrap_r);

    // Configure the height and width accordingly to the last one read
    this->width = width;
    this->height = height;
}

void CubemapTexture2D::bind() const
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->ID);
}
