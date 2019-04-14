#ifndef _MATRICES_H
#define _MATRICES_H

#include <cstdio>
#include <cstdlib>
#include <string>
#include <iostream>

#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace matrix {
    // The Matrix() helps to create matrices using the GLM library.
    // Note that on OpenGL (and GLM) are defined in a column-major fashion,
    // where the matrix elements are stored traveling through their COLUMNS.
    // For example, let
    //
    //       [a b c]
    //   M = [d e f]
    //       [g h i]
    //
    // a 3x3 matrix. In memory, in the column-major OpenGL representation, this
    // matrix is represented by the following array:
    //
    //   M[] = {  a,d,g,    b,e,h,    c,f,i  };
    //              ^         ^         ^
    //              |         |         |
    //           column 1  column 2  column 3
    //
    // To be able to define matrices in a row-major fashion, a helper Matrix() function
    // computes the transpose of the elements passed as parameters
    glm::mat4 Matrix(
        float m00, float m01, float m02, float m03, // LINHA 1
        float m10, float m11, float m12, float m13, // LINHA 2
        float m20, float m21, float m22, float m23, // LINHA 3
        float m30, float m31, float m32, float m33  // LINHA 4
    );


    // Identity matrix
    glm::mat4 identity_matrix();

    // Translation matrix T. Let p=[px,py,pz,pw] a point and t=[tx,ty,tz,0] a
    // a vector in homogeneous coordinates, defined in a Cartesian coordinates
    // system. Then, the matrix T is defined by the following equality:
    //
    //     T*p = p+t.
    //
    glm::mat4 translate_matrix(float tx, float ty, float tz);
    glm::mat4 translate_matrix(glm::vec3 t);

    // Scale matrix S, in relation to the coordinates system origin.
    // Let p=[px,py,pz,pw] a point in homogeneous coordinates.
    // Then, the matrix S is defined by the following equality:
    //
    //     S*p = [sx*px, sy*py, sz*pz, pw].
    //
    glm::mat4 scale_matrix(float sx, float sy, float sz);
    glm::mat4 scale_matrix(glm::vec3 s);

    // Rotation matrix R, in relation to the coordinates system origin, and
    // around the X vector axis (base's coordinate system first vector).
    // Let p=[px,py,pz,pw] a point in homogeneous coordinates.
    // Then, the matrix R is defined by the following equality:
    //
    //   R*p = [ px, c*py-s*pz, s*py+c*pz, pw ];
    //
    // where 'c' and 's' are the rotation angle's cosine and sine, respectively.
    glm::mat4 rotate_x_matrix(float angle);

    // Rotation matrix R, in relation to the coordinates system origin, and
    // around the Y vector axis (base's coordinate system second vector).
    // Let p=[px,py,pz,pw] a point in homogeneous coordinates.
    // Then, the matrix R is defined by the following equality:
    //
    //   R*p = [ c*px+s*pz, py, -s*px+c*pz, pw ];
    //
    // where 'c' and 's' are the rotation angle's cosine and sine, respectively.
    glm::mat4 rotate_y_matrix(float angle);

    // Rotation matrix R, in relation to the coordinates system origin, and
    // around the Z vector axis (base's coordinate system third vector).
    // Let p=[px,py,pz,pw] a point in homogeneous coordinates.
    // Then, the matrix R is defined by the following equality:
    //
    //   R*p = [ c*px-s*py, s*px+c*py, pz, pw ];
    //
    // where 'c' and 's' are the rotation angle's cosine and sine, respectively.
    glm::mat4 rotate_z_matrix(float angle);

    // Rotation matrix R, in relation to the coordinates system origin, and
    // around a vector defined by the parameter `axis`.
    // Tris matrix can be defined by the Rodrigues' formula.
    // Remember that the vector which defines the rotation axis must be normalized.
    glm::mat4 rotate_matrix(float angle, glm::vec4 axis);

    // Function which computer a vector's, with its coefficients defined in any orthonormal base,
    // Euclidian norm
    float norm(glm::vec3 v);
    float norm(glm::vec4 v);

    // Function which normalizes a vector.
    // This means that, let v a vector, norm(v) = 1
    glm::vec3 normalize(glm::vec3 v);
    glm::vec4 normalize(glm::vec4 v);;

    // Cross product between two vectors u and v defined in a
    // orthonormal coordinates system
    glm::vec4 crossproduct(glm::vec4 u, glm::vec4 v);

    // Dot product between two vectors u and v defined in a
    // orthonormal coordinates system
    float dotproduct(glm::vec4 u, glm::vec4 v);

    // Change of Coordinates Matrix to the Camera coordinates system
    glm::mat4 camera_view_matrix(glm::vec4 position_c, glm::vec4 view_vector, glm::vec4 up_vector);

    // Parallel Orthographic Projection Matrix
    glm::mat4 orthographic_matrix(float l, float r, float b, float t, float n, float f);

    // Perspective Projection Matrix
    glm::mat4 perspective_matrix(float field_of_view, float aspect, float n, float f);

    // Prints a Matrix M to the terminal
    void print_matrix(glm::mat4 M, std::string name = " ");

    // Prints a Vector V to the terminal
    void print_vector(glm::vec4 v, std::string name = " ");
}

#endif // _MATRICES_H
