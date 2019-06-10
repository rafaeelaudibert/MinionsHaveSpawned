#include "engine/resource_manager.hpp"
#include "engine/matrices.hpp"
#include "model/cube.hpp"

void Cube::build()
{
    // Initialize shaders
    this->shader = ResourceManager::load_shader("../../src/shaders/cube.vs", "../../src/shaders/cube.fs", nullptr, this->name);

    // Vertex definition
    GLfloat vertices[] =
    {
        //    X      Y     Z     W
        -0.5f, 0.5f, 0.5f, 1.0f,
        -0.5f, -0.5f, 0.5f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f,
        -0.5f, 0.5f, -0.5f, 1.0f,
        -0.5f, -0.5f, -0.5f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f,
    };

    // VBO and VAO
    unsigned int VBO; // VAO is declared in the object
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(this->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Vertexes are in ==> location = 0
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    // Indexes definition
    GLuint model_coefficients[] =
    {
        // We define the vertex model_coefficients which define the cube faces using
        // 12 triangles which will be drawn with the GL_TRIANGLES rendering technique
        0, 1, 2,
        7, 6, 5,
        3, 2, 6,
        4, 0, 3,
        4, 5, 1,
        1, 5, 6,
        0, 2, 3,
        7, 5, 4,
        3, 6, 7,
        4, 3, 7,
        4, 1, 0,
        1, 6, 2,
    };

    // We create an OpenGL bugger to store the indexes above
    // We "turn on" the buffer. Notice that we use GL_ELEMENT_ARRAY_BUFFER now.
    // We allocate memory to the buffer, and copy the data to it.
    GLuint indexes_id;
    glGenBuffers(1, &indexes_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexes_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(model_coefficients), model_coefficients, GL_STATIC_DRAW);

    // Drawing methods definition
    this->drawMode = GL_TRIANGLES;
    this->indexesLength = sizeof(model_coefficients) / sizeof(GLuint);
    this->indexesOffset = 0;

    return;
}

void Cube::render(glm::mat4 view, glm::mat4 projection)
{

    // Set to use this shader
    this->shader.use();

    // Configure view and projection matrices
    this->shader.set_matrix("view", view);
    this->shader.set_matrix("projection", projection);

    // Bind the VAO
    glBindVertexArray(this->VAO);

    // Calculate the model matrix, initializing with
    glm::mat4 model = matrix::translate_matrix(this->position) * matrix::scale_matrix(this->scale) * matrix::rotate_matrix(this->angle, this->orientation);
    this->shader.set_matrix("model", model);

    // Draw the element
    glDrawElements(this->drawMode, this->indexesLength, GL_UNSIGNED_INT, this->indexesOffset);

    // Unbind the VAO to prevent bugs
    glBindVertexArray(0);

}
