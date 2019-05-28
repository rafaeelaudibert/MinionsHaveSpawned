#ifndef CUBEMAP_TEXTURE_H
#define CUBEMAP_TEXTURE_H

#include <glad/glad.h>
#include <vector>
#include <string>

// Texture library
#include <stb_image.h>

// CubemapTexture2D is able to store and configure a cubemap texture in OpenGL.
// It also hosts utility functions for easy management.
class CubemapTexture2D
{
public:
    // Holds the ID of the texture object, used for all texture operations to reference to this particlar texture
    GLuint ID;

    // Texture image dimensions
    GLuint width, height; // Width and height of loaded image in pixels

    // Texture Format
    GLuint internal_format; // Format of texture object
    GLuint image_format;    // Format of loaded image

    // Texture configuration
    GLuint wrap_s;     // Wrapping mode on S axis
    GLuint wrap_t;     // Wrapping mode on T axis
    GLuint wrap_r;     // Wrapping mode on T axis
    GLuint filter_min; // Filtering mode if texture pixels < screen pixels
    GLuint filter_max; // Filtering mode if texture pixels > screen pixels

    // Constructor (sets default texture modes)
    CubemapTexture2D();

    // Generates texture from image data
    void generate(std::vector<std::string> faces);

    // Binds the texture as the current active GL_TEXTURE_CUBE_MAP texture object
    void bind() const;
};

#endif
