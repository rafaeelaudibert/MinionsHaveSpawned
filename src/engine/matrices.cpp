#include "engine\matrices.hpp"

// Transposed matrix
glm::mat4 matrix::Matrix(
    float m00, float m01, float m02, float m03, // LINHA 1
    float m10, float m11, float m12, float m13, // LINHA 2
    float m20, float m21, float m22, float m23, // LINHA 3
    float m30, float m31, float m32, float m33  // LINHA 4
)
{
    return glm::mat4(
        m00, m10, m20, m30, // COLUNA 1
        m01, m11, m21, m31, // COLUNA 2
        m02, m12, m22, m32, // COLUNA 3
        m03, m13, m23, m33  // COLUNA 4
    );
}


// Identity matrix
glm::mat4 matrix::identity_matrix()
{
    return Matrix(
        1.0f , 0.0f , 0.0f , 0.0f , // LINHA 1
        0.0f , 1.0f , 0.0f , 0.0f , // LINHA 2
        0.0f , 0.0f , 1.0f , 0.0f , // LINHA 3
        0.0f , 0.0f , 0.0f , 1.0f   // LINHA 4
    );
}

// Translate matrix with parameters separated
glm::mat4 matrix::translate_matrix(float tx, float ty, float tz)
{
    return Matrix(
        1.0f , 0.0f , 0.0f , tx ,  // LINHA 1
        0.0f , 1.0f , 0.0f , ty ,  // LINHA 2
        0.0f , 0.0f , 1.0f , tz ,  // LINHA 3
        0.0f , 0.0f , 0.0f , 1.0f    // LINHA 4
    );
}

// Translate matrix with parameters as a vector
glm::mat4 matrix::translate_matrix(glm::vec3 t)
{
    return Matrix(
        1.0f , 0.0f , 0.0f , t.x ,  // LINHA 1
        0.0f , 1.0f , 0.0f , t.y ,  // LINHA 2
        0.0f , 0.0f , 1.0f , t.z ,  // LINHA 3
        0.0f , 0.0f , 0.0f , 1.0f    // LINHA 4
    );
}

// Scale matrix with parameters separated
glm::mat4 matrix::scale_matrix(float sx, float sy, float sz)
{
    return Matrix(
        sx   , 0.0f , 0.0f , 0.0f ,  // LINHA 1
        0.0f , sy   , 0.0f , 0.0f ,  // LINHA 2
        0.0f , 0.0f , sz   , 0.0f ,  // LINHA 3
        0.0f , 0.0f , 0.0f , 1.0f    // LINHA 4
    );
}

// Scale matrix with parameters as a vector
glm::mat4 matrix::scale_matrix(glm::vec3 s)
{
    return Matrix(
        s.x  , 0.0f , 0.0f , 0.0f ,  // LINHA 1
        0.0f , s.y  , 0.0f , 0.0f ,  // LINHA 2
        0.0f , 0.0f , s.z  , 0.0f ,  // LINHA 3
        0.0f , 0.0f , 0.0f , 1.0f    // LINHA 4
    );
}

// Rotate matrix around the X axis
glm::mat4 matrix::rotate_x_matrix(float angle)
{
    float c = cos(angle);
    float s = sin(angle);

    return Matrix(
        1.0f , 0.0f , 0.0f , 0.0f ,  // LINHA 1
        0.0f , c    , -s   , 0.0f ,  // LINHA 2
        0.0f , s    , c    , 0.0f ,  // LINHA 3
        0.0f , 0.0f , 0.0f , 1.0f    // LINHA 4
    );
}

// Rotate matrix around the X axis
glm::mat4 matrix::rotate_y_matrix(float angle)
{
    float c = cos(angle);
    float s = sin(angle);

    return Matrix(
        c    , 0.0f , s    , 0.0f ,  // LINHA 1
        0.0f , 1.0f , 0.0f , 0.0f ,  // LINHA 2
        -s   , 0.0f , c    , 0.0f ,  // LINHA 3
        0.0f , 0.0f , 0.0f , 1.0f    // LINHA 4
    );
}

// Rotate matrix around the X axis
glm::mat4 matrix::rotate_z_matrix(float angle)
{
    float c = cos(angle);
    float s = sin(angle);

    return Matrix(
        c    , -s   , 0.0f , 0.0f ,  // LINHA 1
        s    , c    , 0.0f , 0.0f ,  // LINHA 2
        0.0f , 0.0f , 1.0f , 0.0f ,  // LINHA 3
        0.0f , 0.0f , 0.0f , 1.0f    // LINHA 4
    );
}

// Rotate matrix around an arbitrary axis
glm::mat4 matrix::rotate_matrix(float angle, glm::vec4 axis)
{
    float c = cos(angle);
    float s = sin(angle);
    float minusc = 1 - c;

    glm::vec4 v = normalize(axis);

    float vx = v.x;
    float vy = v.y;
    float vz = v.z;

    // Rodrigues' Formula
    return Matrix(
        (vx * vx * minusc) + c        , (vx * vy * minusc) - (vz * s) , (vx * vz * minusc) + (vy * s) , 0.0f ,  // LINHA 1
        (vx * vy * minusc) + (vz * s) , (vy * vy * minusc) + c        , (vy * vz * minusc) - (vx * s) , 0.0f ,  // LINHA 2
        (vx * vz * minusc) - (vy * s) , (vy * vz * minusc) + (vx * s) , (vz * vz * minusc) + c        , 0.0f ,  // LINHA 3
        0.0f                          , 0.0f                          , 0.0f                          , 1.0f    // LINHA 4
    );
}

// Norm for a vec4
float matrix::norm(glm::vec4 v)
{
    float vx = v.x;
    float vy = v.y;
    float vz = v.z;
    float vw = v.w;

    return sqrt( vx*vx + vy*vy + vz*vz + vw*vw );
}

// Norm for a vec3
float matrix::norm(glm::vec3 v)
{
    float vx = v.x;
    float vy = v.y;
    float vz = v.z;

    return sqrt( vx*vx + vy*vy + vz*vz );
}

// Normalize a vec4 length
glm::vec4 matrix::normalize(glm::vec4 v)
{
    return v / norm(v);
}

// Normalize a vec3 length
glm::vec3 matrix::normalize(glm::vec3 v)
{
    return v / norm(v);
}

// Cross product between vectors u and v
glm::vec4 matrix::crossproduct(glm::vec4 u, glm::vec4 v)
{
    float u1 = u.x;
    float u2 = u.y;
    float u3 = u.z;
    float v1 = v.x;
    float v2 = v.y;
    float v3 = v.z;

    return glm::vec4(
        u2*v3 - u3*v2,
        u3*v1 - u1*v3,
        u1*v2 - u2*v1,
        0.0f
    );
}

// Dot product between vectors u and v
float matrix::dotproduct(glm::vec4 u, glm::vec4 v)
{
    float u1 = u.x;
    float u2 = u.y;
    float u3 = u.z;
    float u4 = u.w;
    float v1 = v.x;
    float v2 = v.y;
    float v3 = v.z;
    float v4 = v.w;

    if ( u4 != 0.0f || v4 != 0.0f )
    {
        fprintf(stderr, "ERROR: Produto escalar n�o definido para pontos.\n");
        std::exit(EXIT_FAILURE);
    }

    return u1*v1 + u2*v2 + u3*v3 + u4*v4;
}

// Change of Coordinates Matrix to the Camera coordinates system
glm::mat4 matrix::camera_view_matrix(glm::vec4 position_c, glm::vec4 view_vector, glm::vec4 up_vector)
{
    glm::vec4 w = normalize(-view_vector);
    glm::vec4 u = normalize(crossproduct(up_vector, w));

    glm::vec4 v = crossproduct(w,u);

    glm::vec4 origin_o = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

    return Matrix(
        u.x   , u.y   , u.z   , dotproduct(-u, position_c - origin_o) ,  // LINHA 1
        v.x   , v.y   , v.z   , dotproduct(-v, position_c - origin_o) ,  // LINHA 2
        w.x   , w.y   , w.z   , dotproduct(-w, position_c - origin_o) ,  // LINHA 3
        0.0f , 0.0f , 0.0f , 1.0f    // LINHA 4
    );
}

// Parallel Orthographic Projection Matrix
glm::mat4 matrix::orthographic_matrix(float l, float r, float b, float t, float n, float f)
{
    glm::mat4 M = Matrix(
        2 / (r - l) , 0.0f        , 0.0f        , -(r + l) / ( r - l ) ,  // LINHA 1
        0.0f        , 2 / (t - b) , 0.0f        , -(t + b) / ( t - b ) ,  // LINHA 2
        0.0f        , 0.0f        , 2 / (f - n) , -(f + n) / ( f - n ) ,  // LINHA 3
        0.0f        , 0.0f        , 0.0f        , 1.0f    // LINHA 4
    );

    return M;
}

// Perspective Projection Matrix
glm::mat4 matrix::perspective_matrix(float field_of_view, float aspect, float n, float f)
{
    float t = fabs(n) * tanf(field_of_view / 2.0f);
    float b = -t;
    float r = t * aspect;
    float l = -r;

    glm::mat4 P = Matrix(
        n    , 0.0f , 0.0f  , 0.0f   ,  // LINHA 1
        0.0f , n    , 0.0f  , 0.0f   ,  // LINHA 2
        0.0f , 0.0f , n + f , -f * n ,  // LINHA 3
        0.0f , 0.0f , 1.0f  , 0.0f    // LINHA 4
    );

    // The matrix M is the same computed in a orthographic projection
    glm::mat4 M = orthographic_matrix(l, r, b, t, n, f);

    return -M*P;
}

// Print a matrix
void matrix::print_matrix(glm::mat4 M, std::string name)
{
    std::cout << name << std::endl;
    printf("[ %+0.2f  %+0.2f  %+0.2f  %+0.2f ]\n", M[0][0], M[1][0], M[2][0], M[3][0]);
    printf("[ %+0.2f  %+0.2f  %+0.2f  %+0.2f ]\n", M[0][1], M[1][1], M[2][1], M[3][1]);
    printf("[ %+0.2f  %+0.2f  %+0.2f  %+0.2f ]\n", M[0][2], M[1][2], M[2][2], M[3][2]);
    printf("[ %+0.2f  %+0.2f  %+0.2f  %+0.2f ]\n", M[0][3], M[1][3], M[2][3], M[3][3]);
}

void matrix::print_vector(glm::vec4 v, std::string name)
{
    std::cout << name << std::endl;
    printf("[ %+0.2f ]\n", v[0]);
    printf("[ %+0.2f ]\n", v[1]);
    printf("[ %+0.2f ]\n", v[2]);
    printf("[ %+0.2f ]\n", v[3]);
}
