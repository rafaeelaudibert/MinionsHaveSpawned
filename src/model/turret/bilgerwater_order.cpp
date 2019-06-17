#include "engine/resource_manager.hpp"
#include "engine/matrices.hpp"
#include "model/turret/bilgerwater_order.hpp"

void BilgerwaterOrder::build()
{
    // Initialize object, shaders, and textures
    ResourceManager::load_object("../../src/objects/bw_order_turret.obj", this, this->name);
    this->shader = ResourceManager::load_shader("../../src/shaders/default_texture.vs", "../../src/shaders/default_texture.fs", nullptr, this->name);
    this->shader.use();

    switch (this->color)
    {
    case TurretColor::RED:
    case TurretColor::BLUE:
        this->texture = ResourceManager::load_texture("../../src/textures/bw_order_turret.jpg", this->name);
        break;
    default:
        throw std::runtime_error("[ERROR] Error when trying to load the BilgerwaterOrder model - Non existent TurretColor.");
    }

    return;
}

void BilgerwaterOrder::render(glm::mat4 view, glm::mat4 projection)
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

