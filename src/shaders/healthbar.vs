#version 330 core

// Atributos de vértice recebidos como entrada ("in") pelo Vertex Shader.
layout (location = 0) in vec4 model_coefficients;
// Matrizes computadas no código C++ e enviadas para a GPU
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// Atributos de vértice que serão gerados como saída ("out") pelo Vertex Shader.
// ** Estes serão interpolados pelo rasterizador! ** gerando, assim, valores
// para cada fragmento, os quais serão recebidos como entrada pelo Fragment
// Shader.
out vec4 color_vertex;
out vec4 position;

void main()
{
    gl_Position = projection * view * model * model_coefficients;

    position = model_coefficients / 4.0 + 0.5; // Normalize from 0.0 to 1.0
}
