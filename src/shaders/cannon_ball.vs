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
out vec4 color_v;
out vec4 position_model;

void main()
{
    gl_Position = projection * view * model * model_coefficients;

    // We obtain the camera position using the matrix inverse that defines the camera cordinates system
    vec4 origin = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 camera_position = inverse(view) * origin;

    // Current vertex position in the world coordinates system
    vec4 position_world = model * model_coefficients;

    // Current vertex position in the model coordinates system
    position_model = model_coefficients;

    // Current vertex normal in the world coordinates system
    vec4 n = normalize(inverse(transpose(model)) * normal_coefficients);

    // Vector which defines the light direction related to the current vertex.
    // This is a global light.
    vec4 l = normalize(vec4(1.0,1.0,1.0,0.0));

    // Vector which defines the camera direction related to the current vertex.
    vec4 v = normalize(camera_position - position_world);

    // Vector which defines the direction of the specular ideal reflection.
    vec4 r = -l + 2 * n * (dot(l, v));

    // Parameters which define the surface spectral properties.
    vec4 Kd = vec4(1.0, 1.0, 1.0, 1.0); // Diffuse reflectance // Orange color = 0.97, 0.67, 0.03, 1.0
    vec4 Ks = vec4(1.0, 1.0, 1.0, 1.0); // Specular reflectance
    vec4 Ka = vec4(1.0, 1.0, 1.0, 1.0); // Ambient reflectance
    float q = 80.0; // Specular exponent for Blinn-Phong's illumination model

    // Parameters which define the illumination spectrum
    vec4 I  = vec4(1.0, 1.0, 1.0, 1.0); // Light source spectrum
    vec4 Ia = vec4(0.4, 0.4, 0.4, 1.0); // Ambient source light spectrum

    // Diffuse term used in Lambert's cosine law
    vec4 lambert_diffuse_term = Kd * I * max(0, dot(n, l));

    // Ambient term
    vec4 ambient_term = Ka * Ia;

    // Specular term used in Blinn Phong's illumination model
    vec4 h = normalize(v + l);
    vec4 phong_specular_term  = Ks * I * pow(max(0, dot(n, h)), q);

    // Final fragment color calculated with a combination of the diffuse, specular and ambiance terms.
    color_v = lambert_diffuse_term + ambient_term + phong_specular_term;
}
