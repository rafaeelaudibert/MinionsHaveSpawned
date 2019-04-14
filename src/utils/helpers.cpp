#include <cstdio>
#include <algorithm>
#include "utils\helpers.hpp"

int utils::clamping(int value, int maximum, int minimum)
{
    return std::min(maximum, std::max(minimum, value));
}

float utils::clamping(float value, float maximum, float minimum)
{
    return std::min(maximum, std::max(minimum, value));
}

double utils::clamping(double value, double maximum, double minimum)
{
    return std::min(maximum, std::max(minimum, value));
}

void utils::print_gpu_info()
{

    const GLubyte *vendor = glGetString(GL_VENDOR);
    const GLubyte *renderer = glGetString(GL_RENDERER);
    const GLubyte *glversion = glGetString(GL_VERSION);
    const GLubyte *glslversion = glGetString(GL_SHADING_LANGUAGE_VERSION);

    printf("GPU: %s, %s, OpenGL %s, GLSL %s\n", vendor, renderer, glversion, glslversion);
}
