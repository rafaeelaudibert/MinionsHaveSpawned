#version 330 core

// Atributos de vértice recebidos como entrada ("in") pelo Vertex Shader.
// Veja a função BuildTrianglesAndAddToVirtualScene() em "main.cpp".
layout (location = 0) in vec4 model_coefficients;
layout (location = 1) in vec4 normal_coefficients;
layout (location = 2) in vec2 texture_coefficients;

// Matrizes computadas no código C++ e enviadas para a GPU
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// Atributos de vértice que serão gerados como saída ("out") pelo Vertex Shader.
// ** Estes serão interpolados pelo rasterizador! ** gerando, assim, valores
// para cada fragmento, os quais serão recebidos como entrada pelo Fragment
// Shader. Veja o arquivo "shader_fragment.glsl".
out vec4 position_world;
out vec4 position_model;
out vec4 normal;
out vec2 tex_coords;

void main()
{
    gl_Position = projection * view * model * model_coefficients;

    // Current vertex position in the world coordinates system
    position_world = model * model_coefficients;

    // Current vertex position in the model coordinates system
    position_model = model_coefficients;

    // Send the normal
    normal = inverse(transpose(model)) * normal_coefficients;
    normal.w = 0.0;

    // Send the texture coordinates
    tex_coords = texture_coefficients;
}
