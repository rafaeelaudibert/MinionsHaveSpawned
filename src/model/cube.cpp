#include "engine/resource_manager.hpp"
#include "engine/matrices.hpp"
#include "model/cube.hpp"

void Cube::build()
{
    // Initialize shaders
    this->shader = ResourceManager::load_shader("../../src/shaders/default_lightning.vs", "../../src/shaders/default_lightning.fs", nullptr, this->name);
    ResourceManager::load_object("../../src/objects/cube.obj", this, this->name);

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
    glm::mat4 model = matrix::identity_matrix(); // make sure to initialize matrix to identity matrix first
    model *= matrix::translate_matrix(this->position);
    model *= matrix::scale_matrix(this->scale);
    model *= matrix::rotate_matrix(this->angle, this->orientation);
    this->shader.set_matrix("model", model);

    // Draw the element
    glDrawElements(this->drawMode, this->indexesLength, GL_UNSIGNED_INT, this->indexesOffset);

    // Unbind the VAO to prevent bugs
    glBindVertexArray(0);

}
