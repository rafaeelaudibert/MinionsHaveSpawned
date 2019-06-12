#include "engine/resource_manager.hpp"
#include "engine/matrices.hpp"
#include "model/cube.hpp"

void Cube::build()
{
    // Load object
    ResourceManager::load_object("../../src/objects/cube.obj", this, this->name);

    // Initialize shaders
    this->shader = ResourceManager::load_shader("../../src/shaders/default_lighting.vs", "../../src/shaders/default_lighting.fs", nullptr, this->name);
    this->shader.use();

    return;
}

void Cube::build(const GLchar* texture_path, const GLchar* texture_name) {
    // Load object
    ResourceManager::load_object("../../src/objects/cube.obj", this, this->name);

    // Initialize shaders
    this->shader = ResourceManager::load_shader("../../src/shaders/default_texture.vs", "../../src/shaders/default_texture.fs", nullptr, this->name);
    this->shader.use();

    // Adds texture
    this->texture = ResourceManager::load_texture(texture_path, texture_name);

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

    // Bind textures
    if (&this->texture != NULL)
        this->texture.bind();

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
