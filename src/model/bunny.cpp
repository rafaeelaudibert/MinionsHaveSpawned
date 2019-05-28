#include "engine/resource_manager.hpp"
#include "engine/matrices.hpp"
#include "model/bunny.hpp"

void Bunny::build()
{
    // Initialize shaders
    this->shader = ResourceManager::load_shader("../../src/shaders/bunny.vs", "../../src/shaders/bunny.fs", nullptr, this->name);
    ResourceManager::load_object("../../src/objects/bunny.obj", this, "bunny");

    return;
}

void Bunny::render(glm::mat4 view, glm::mat4 projection)
{

    // Set to use this shader
    this->shader.use();

    // Configure view and projection matrices
    this->shader.set_matrix("view", view);
    this->shader.set_matrix("projection", projection);

    // Bind the VAO
    glBindVertexArray(this->VAO);

    // Calculate the model matrix
    glm::mat4 model = matrix::identity_matrix(); // make sure to initialize matrix to identity matrix first
    model *= matrix::translate_matrix(this->position);
    model *= matrix::rotate_matrix(this->angle, this->orientation);
    this->shader.set_matrix("model", model);

    // Draw the element
    glDrawElements(this->drawMode, this->indexesLength, GL_UNSIGNED_INT, this->indexesOffset);

    // Unbind the VAO to prevent bugs
    glBindVertexArray(0);

}
