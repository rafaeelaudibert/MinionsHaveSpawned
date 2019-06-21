#include "engine/resource_manager.hpp"
#include "engine/matrices.hpp"
#include "model/enemy/order_super.hpp"

void OrderSuper::build()
{
    // Initialize object, shaders, and textures
    ResourceManager::load_object("../../src/objects/order_minion_super.obj", this, this->name);
    this->shader = ResourceManager::load_shader("../../src/shaders/enemy.vs", "../../src/shaders/enemy.fs", nullptr, this->name);
    this->shader.use();

    switch (this->color)
    {
    case EnemyColor::RED:
        this->texture = ResourceManager::load_texture("../../src/textures/order_minion_super_red.jpg", this->name);
        break;
    case EnemyColor::BLUE:
        this->texture = ResourceManager::load_texture("../../src/textures/order_minion_super_blue.jpg", this->name);
        break;
    default:
        throw std::runtime_error("[ERROR] Error when trying to load the OrderSuper model - Non existent EnemyColor.");
    }

    return;
}

void OrderSuper::render(glm::mat4 view, glm::mat4 projection)
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

    // Render the dead minions properly
    if (this->is_dead())
        shader.set("is_dead", true);

    // Draw the element
    glDrawElements(this->drawMode, this->indexesLength, GL_UNSIGNED_INT, this->indexesOffset);

    // Unbind the VAO to prevent bugs
    glBindVertexArray(0);

    // Render health bar, calculating the percentage of life
    // this->health_bar->render(view, projection, this->current_life_points / this->max_life_points);
}

void OrderSuper::update(float delta_time, Camera *camera) {
    // this->position.y += 0.1 * delta_time;
}
