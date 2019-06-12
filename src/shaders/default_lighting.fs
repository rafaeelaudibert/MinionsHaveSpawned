#version 330 core

// Fragment attributes received as input by the Fragment Shader
// This attribute was generated by the rasterizer with
// each vertex color interpolation defined in "bunny.vs" and in "main.cpp".
in vec4 position_world;
in vec4 normal;

// Posi��o do v�rtice atual no sistema de coordenadas local do modelo.
in vec4 position_model;

// Current vertex position in the world coordinates system
in vec2 texcoords;

// Matrices computed in C++ code and sent to the GPU
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


// Vari�veis para acesso das imagens de textura
uniform sampler2D TextureImage0;


// The Fragment Shader output is the final fragment color
out vec3 color;

void main()
{
    // We obtain the camera position using the matrix inverse that defines the camera cordinates system
    vec4 origin = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 camera_position = inverse(view) * origin;

    // The current fragment is covered by a point which belongs to the surface of
    // one of the scene's virtual objects. This point, p, has a position in the
    // world coordinates system. This position is obtained through the interpolation,
    // done by the rasterizer, of each vertex position.
    vec4 p = position_world;

    // Current fragment normal, interpolated by the rasterizer through each vertex normal.
    vec4 n = normalize(normal);

    // Vector which defines the light direction related to the current point.
    // This is a global light.
    vec4 l = normalize(vec4(1.0,1.0,1.0,0.0));

    // Vector which defines the camera direction related to the current point.
    vec4 v = normalize(camera_position - p);

    // Vector which defines the direction of the specular ideal reflection.
    vec4 r = -l + 2 * n * (dot(l, v));

    // Parameters which define the surface spectral properties.
    vec3 Kd = vec3(0.08,0.4,0.3); // Diffuse reflectance
    vec3 Ks = vec3(0.8,0.8,0.8); // Specular reflectance
    vec3 Ka = vec3(0.04,0.2,0.4); // Ambient reflectance
    float q = 5.0; // Specular exponent for Phong's illumination model

    // Coordenadas de textura U e V
    float U = 0.0;
    float V = 0.0;

    // Parameters which define the illumination spectrum
    vec3 I = vec3(1.0,1.0,1.0); // Light source spectrum
    vec3 Ia = vec3(0.2,0.2,0.2); // Ambient source light spectrum

    // Diffuse term used in Lambert's cosine law
    vec3 lambert_diffuse_term = Kd * I * max(0, dot(n, l));

    // Ambient term
    vec3 ambient_term = Ka * Ia;

    // Specular term used in Phong's illumination model
    vec4 h = normalize(v + l);
    vec3 phong_specular_term  = Ks * I * pow(max(0, dot(n, h)), q);

    // Final fragment color calculated with a combination of the diffuse, specular and ambiance terms.
    color = lambert_diffuse_term + ambient_term + phong_specular_term;

    // We need to make our gamma color correction, considering a sRGB display.
    // See https://en.wikipedia.org/w/index.php?title=Gamma_correction&oldid=751281772#Windows.2C_Mac.2C_sRGB_and_TV.2Fvideo_standard_gammas
    color = pow(color, vec3(1.0,1.0,1.0)/2.2);
}