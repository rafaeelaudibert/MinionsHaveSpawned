#include "engine/resource_manager.hpp"
#include "engine/matrices.hpp"
#include "model/health_bar.hpp"
#include "engine/game.hpp"

void HealthBar::build()
{
    // Initialize shaders
    this->shader = ResourceManager::load_shader("../../src/shaders/healthbar.vs", "../../src/shaders/healthbar.fs", nullptr, this->name);
    this->shader.use();
    this->shader.set("health_percentage", 1.0f);

    // VAO and VBOS
    glGenVertexArrays(1, &this->VAO); // VAO is declared in the object

    // Vertex definition
    float vertex_coefficients[] =
    {
        -2.0f, -0.2f,  0.0f,
        -2.0f,  0.2f,  0.0f,
        2.0f, -0.2f,  0.0f,
        2.0f,  0.2f,  0.0f
    };

    GLuint VBOVertex;
    glGenBuffers(1, &VBOVertex);

    glBindVertexArray(this->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBOVertex);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_coefficients), &vertex_coefficients, GL_STATIC_DRAW);

    // Vertexes are in location = 0
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

    // Now we define the indexes
    GLuint indexes[] = {
        0, 3, 1,    // Triangle 1
        0, 2, 3     // Triangle 2
    };

    // Create an OpenGL buffer to store the indexes
    GLuint indexes_id;
    glGenBuffers(1, &indexes_id);

    // "Turn on" the buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexes_id);

    // Allocate memory for the buffer, and we store the indexes in there
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexes), &indexes, GL_STATIC_DRAW);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // XXX Wrong!

    // "Turn off" the VAO
    glBindVertexArray(0);

    // Drawing methods definition
    this->drawMode = GL_TRIANGLES;
    this->indexesLength = sizeof(indexes) / sizeof(float);
    this->indexesOffset = 0;

    return;
}

void HealthBar::render(glm::mat4 view, glm::mat4 projection) {
    this->render(view, projection, 0.8f);
}

void HealthBar::render(glm::mat4 view, glm::mat4 projection, float health_percentage)
{

    // Get the camera position
    glm::vec4 camera_position = Game::camera.position;

    // Set to use this shader
    this->shader.use();

    // Configure life
    this->shader.set("health_percentage", health_percentage);

    // Configure view and projection matrices
    this->shader.set_matrix("view", view);
    this->shader.set_matrix("projection", projection);

    // Bind the VAO
    glBindVertexArray(this->VAO);

    // Calculate the model matrix
    glm::mat4 model = matrix::identity_matrix(); // make sure to initialize matrix to identity matrix first
    model *= matrix::translate_matrix(this->position);
    model *= matrix::scale_matrix(this->scale);
    model *= matrix::rotate_matrix(std::atan2(camera_position.x - this->position.x, camera_position.z - this->position.z), this->orientation);
    this->shader.set_matrix("model", model);

    // Draw the element
    glDrawElements(this->drawMode, this->indexesLength, GL_UNSIGNED_INT, this->indexesOffset);

    // Unbind the VAO to prevent bugs
    glBindVertexArray(0);
}
