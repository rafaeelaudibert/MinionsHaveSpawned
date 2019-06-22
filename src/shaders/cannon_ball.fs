#version 330 core

// Matrices computed in C++ code and sent to the GPU
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// Vari�veis para acesso das imagens de textura
uniform sampler2D texture_1;

// To compute the U and V coordinates of the texture
uniform vec4 bbox_min;
uniform vec4 bbox_max;

// The Vertex Shader computed color (with Gouraud shading)
in vec4 color_v;

// Position of this vertex
in vec4 position_model;

// The Fragment Shader output is the final fragment color
out vec4 color;

// Constants for PI and PI/2
#define M_PI   3.14159265358979323846
#define M_PI_2 1.57079632679489661923

void main()
{

    // Compute U and V texture coordinates for the sphere
    vec4 p_center = position_model - ((bbox_min + bbox_max) / 2.0);

    float ro = length(p_center);
    float phi = asin(p_center.y / ro);
    float theta = atan(p_center.x, p_center.z);

    float U = (theta + M_PI) / (2 * M_PI);
    float V = (phi + M_PI_2) / M_PI;

    // Texture coloring
    color = texture(texture_1, vec2(U, V)) * color_v;
}
