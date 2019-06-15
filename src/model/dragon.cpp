#include "engine/resource_manager.hpp"
#include "engine/matrices.hpp"
#include "model/dragon.hpp"

void Dragon::build()
{
    // Initialize object, shaders, and textures
    this->shader = ResourceManager::load_shader("../../src/shaders/default_texture.vs", "../../src/shaders/default_texture.fs", nullptr, this->name);
    this->shader.use();

    switch (this->type)
    {
    case DragonType::AIR:
        ResourceManager::load_object("../../src/objects/dragon.obj", this, this->name);
        this->texture = ResourceManager::load_texture("../../src/textures/dragon_air.jpg", this->name);
        break;
    case DragonType::EARTH:
        ResourceManager::load_object("../../src/objects/dragon.obj", this, this->name);
        this->texture = ResourceManager::load_texture("../../src/textures/dragon_earth.jpg", this->name);
        break;
    case DragonType::FIRE:
        ResourceManager::load_object("../../src/objects/dragon.obj", this, this->name);
        this->texture = ResourceManager::load_texture("../../src/textures/dragon_fire.jpg", this->name);
        break;
    case DragonType::WATER:
        ResourceManager::load_object("../../src/objects/dragon.obj", this, this->name);
        this->texture = ResourceManager::load_texture("../../src/textures/dragon_water.jpg", this->name);
        break;
    case DragonType::ELDER:
        ResourceManager::load_object("../../src/objects/elder_dragon.obj", this, this->name);
        this->texture = ResourceManager::load_texture("../../src/textures/dragon_elder.jpg", this->name);
        break;
    default:
        throw std::runtime_error("[ERROR] Error when trying to load the Dragon model - Non existent DragonType.");
    }

    return;
}

void Dragon::render(glm::mat4 view, glm::mat4 projection)
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
