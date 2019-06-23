#include "engine/resource_manager.hpp"
#include "engine/matrices.hpp"
#include "model/enemy/chaos_siege.hpp"

void ChaosSiege::build()
{
    // Initialize object, shaders, and textures
    ResourceManager::load_object("../../src/objects/chaos_minion_siege.obj", this, this->name);
    this->shader = ResourceManager::load_shader("../../src/shaders/enemy.vs", "../../src/shaders/enemy.fs", nullptr, this->name);
    this->shader.use();

    switch (this->color)
    {
    case EnemyColor::RED:
        this->texture = ResourceManager::load_texture("../../src/textures/chaos_minion_siege_red.jpg", this->name);
        break;
    case EnemyColor::BLUE:
        this->texture = ResourceManager::load_texture("../../src/textures/chaos_minion_siege_blue.jpg", this->name);
        break;
    default:
        throw std::runtime_error("[ERROR] Error when trying to load the ChaosSiege model - Non existent EnemyColor.");
    }

    return;
}

void ChaosSiege::render(glm::mat4 view, glm::mat4 projection)
{

    // Set to use this shader
    this->shader.use();

    // Configure view, projection and model matrices
    this->shader.set_matrix("view", view);
    this->shader.set_matrix("projection", projection);
    this->shader.set_matrix("model", this->model_matrix());

    // Bind textures
    this->texture.bind();

    // Bind the VAO
    glBindVertexArray(this->VAO);

    // Render the dead minions properly
    if (this->is_dead())
        shader.set("is_dead", true);

    // Draw the element
    glDrawElements(this->drawMode, this->indexesLength, GL_UNSIGNED_INT, this->indexesOffset);

    // Unbind the VAO to prevent bugs
    glBindVertexArray(0);
}
