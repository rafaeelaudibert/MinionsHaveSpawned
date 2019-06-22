#include "engine/resource_manager.hpp"
#include "engine/matrices.hpp"
#include "model/dummy.hpp"

void Dummy::build()
{
    // Initialize object, shaders, and textures
    ResourceManager::load_object("../../src/objects/dummy.obj", this, this->name);
    this->shader = ResourceManager::load_shader("../../src/shaders/cannon_ball.vs", "../../src/shaders/cannon_ball.fs", nullptr, this->name);
    this->shader.use();

    this->texture = ResourceManager::load_texture("../../src/textures/dummy_blue.jpg", this->name);

    return;
}

void Dummy::render(glm::mat4 view, glm::mat4 projection)
{

    // Set to use this shader
    this->shader.use();

    // Configure view and projection matrices
    this->shader.set_matrix("view", view);
    this->shader.set_matrix("projection", projection);

    // Bind textures
    this->texture.bind();

    // Bind the VAO
    glBindVertexArray(this->VAO);

    // Give bounding box to compute texture coordinates
    this->shader.set_vector("bbox_min", glm::vec4(this->bbox_min, 1.0f));
    this->shader.set_vector("bbox_max", glm::vec4(this->bbox_max, 1.0f));

    // Calculate the model matrix
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
