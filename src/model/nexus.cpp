#include "engine/resource_manager.hpp"
#include "engine/matrices.hpp"
#include "model/nexus.hpp"

void Nexus::build()
{
    // Initialize object, shaders, and textures
    switch (this->color)
    {
    case NexusColorSide::CHAOS_BLUE:
    case NexusColorSide::CHAOS_RED:
        ResourceManager::load_object("../../src/objects/chaos_nexus.obj", this, this->name);
        break;
    case NexusColorSide::ORDER_BLUE:
    case NexusColorSide::ORDER_RED:
        ResourceManager::load_object("../../src/objects/order_nexus.obj", this, this->name);
        break;
    default:
        throw std::runtime_error("[ERROR] Error when trying to load the Nexus model - Non existent NexusColorSide.");
    }
    this->shader = ResourceManager::load_shader("../../src/shaders/default_texture.vs", "../../src/shaders/default_texture.fs", nullptr, this->name);
    this->shader.use();

    switch (this->color)
    {
    case NexusColorSide::CHAOS_RED:
        this->texture = ResourceManager::load_texture("../../src/textures/chaos_nexus_red.jpg", this->name);
        break;
    case NexusColorSide::CHAOS_BLUE:
        this->texture = ResourceManager::load_texture("../../src/textures/chaos_nexus_blue.jpg", this->name);
        break;
    case NexusColorSide::ORDER_RED:
        this->texture = ResourceManager::load_texture("../../src/textures/order_nexus_red.jpg", this->name);
        break;
    case NexusColorSide::ORDER_BLUE:
        this->texture = ResourceManager::load_texture("../../src/textures/order_nexus_blue.jpg", this->name);
        break;
    default:
        throw std::runtime_error("[ERROR] Error when trying to load the Nexus model - Non existent NexusColorSide.");
    }

    return;
}

void Nexus::render(glm::mat4 view, glm::mat4 projection)
{
    // If its dead, render its rubble counter part, else, render this
    if (this->is_dead()) {
        this->nexus_rubble->render(view, projection);
    } else {
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
}

bool Nexus::is_dead(){
    return this->current_life_points < 0;
}

void Nexus::hit(float life_points) {
    if (!this->is_dead())
        this->current_life_points -= life_points;

    return;
}

void Nexus::render_health_bar(glm::mat4 view, glm::mat4 projection) {

    // For the nexus, only render the healthbar if it is not dead
    if (!this->is_dead())
        this->health_bar->render(view, projection, this->current_life_points / this->max_life_points);

    return;
}
