#version 330 core

// Matrices computed in C++ code and sent to the GPU
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// Vari�veis para acesso das imagens de textura
uniform sampler2D texture_1;

// Position of this vertex
in vec4 position_model;
in vec4 position_world;

// Normal information
in vec4 normal;

// Texture coordinates
in vec2 tex_coords;

// The Fragment Shader output is the final fragment color
out vec4 color;

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
    vec4 Kd = vec4(1.0, 1.0, 1.0, 1.0); // Diffuse reflectance
    vec4 Ks = vec4(2.0, 2.0, 2.0, 1.0); // Specular reflectance
    vec4 Ka = vec4(1.0, 1.0, 1.0, 1.0); // Ambient reflectance
    float q = 80.0; // Specular exponent for Blinn-Phong's illumination model

    // Parameters which define the illumination spectrum
    vec4 I = vec4(1.0, 1.0, 1.0, 1.0); // Light source spectrum
    vec4 Ia = vec4(0.2, 0.2, 0.2, 1.0); // Ambient source light spectrum

    // Diffuse term used in Lambert's cosine law
    vec4 lambert_diffuse_term = Kd * I * max(0, dot(n, l));

    // Ambient term
    vec4 ambient_term = Ka * Ia;

    // Specular term used in Phong's illumination model
    vec4 h = normalize(v + l);
    vec4 phong_specular_term  = Ks * I * pow(max(0, dot(n, h)), q);

    // Final fragment color calculated with a combination of the diffuse, specular and ambiance terms, multiplied by the texture.
    color = texture(texture_1, tex_coords) * (lambert_diffuse_term + ambient_term + phong_specular_term);

    // We need to make our gamma color correction, considering a sRGB display.
    // See https://en.wikipedia.org/w/index.php?title=Gamma_correction&oldid=751281772#Windows.2C_Mac.2C_sRGB_and_TV.2Fvideo_standard_gammas
    color = pow(color, vec4(1.0, 1.0, 1.0, 1.0) / 2.2);
}
