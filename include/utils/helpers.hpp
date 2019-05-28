#ifndef UTILS_H_
#define UTILS_H_

#include <glad/glad.h>  // Criação de contexto OpenGL 3.3

namespace utils
{

// Clamps a value to be between maximum and minimum
int clamping(int value, int maximum, int minimum);
float clamping(float value, float maximum, float minimum);
double clamping(double value, double maximum, double minimum);

// Prints information about the current GPU
void print_gpu_info();
}
#endif // UTILS_H_
