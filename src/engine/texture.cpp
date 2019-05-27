#include <iostream>

#include "engine/texture.hpp"

Texture2D::Texture2D()
    : width(0), height(0), wrap_s(GL_CLAMP_TO_EDGE), wrap_t(GL_CLAMP_TO_EDGE), filter_min(GL_LINEAR_MIPMAP_LINEAR), filter_max(GL_LINEAR)
{
    glGenTextures(1, &this->texture_ID);
    glGenSamplers(1, &this->sampler_ID);
}


void Texture2D::generate(GLuint width, GLuint height, GLuint sampler_index, unsigned char *data)
{
    this->width = width;
    this->height = height;

    this->sampler_index = sampler_index;

    // Veja slide 160 do documento "Aula_20_e_21_Mapeamento_de_Texturas.pdf"
    glSamplerParameteri(this->sampler_ID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glSamplerParameteri(this->sampler_ID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Parâmetros de amostragem da textura. Falaremos sobre eles em uma próxima aula.
    glSamplerParameteri(this->sampler_ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glSamplerParameteri(this->sampler_ID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Agora enviamos a imagem lida do disco para a GPU
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);

    glActiveTexture(GL_TEXTURE0 + this->sampler_index);
    glBindTexture(GL_TEXTURE_2D, this->texture_ID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindSampler(this->sampler_index, this->sampler_ID);
}

void Texture2D::bind() const
{
    glBindTexture(GL_TEXTURE_2D, this->texture_ID);
}
