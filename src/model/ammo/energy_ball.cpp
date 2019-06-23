#include "engine/resource_manager.hpp"
#include "engine/matrices.hpp"
#include "model/ammo/energy_ball.hpp"

void EnergyBall::build()
{
    // Initialize object, shaders, and textures
    ResourceManager::load_object("../../src/objects/cannon_ball.obj", this, this->name);
    this->shader = ResourceManager::load_shader("../../src/shaders/default_lighting.vs", "../../src/shaders/default_lighting.fs", nullptr, this->name);
    this->shader.use();

    return;
}

void EnergyBall::render(glm::mat4 view, glm::mat4 projection)
{

    if (!this->completed_movement())
    {
        // Set to use this shader
        this->shader.use();

        // Configure view and projection matrices
        this->shader.set_matrix("view", view);
        this->shader.set_matrix("projection", projection);

        // Bind the VAO
        glBindVertexArray(this->VAO);

        // Configure the color
        this->shader.set_vector("color_v", glm::vec4(0.14f, 1.0f, 0.91f, 1.0f));

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
    else
    {
        printf("[WARN] TODO: You should delete me! %s\n", this->name.c_str());
    }

}

// Calculate the linear curve for the ball
void EnergyBall::update(float delta_time)
{
    if (this->completed_movement())     // Already finished moving
    {
        this->hit_enemy();
    }
    else if (this->target != nullptr)     // Keep moving the ball, linearly towards the enemy (as it is moving, it wont be actually linear)
    {
        this->time += delta_time * this->speed;

        glm::vec4 p_origin = this->origin_position;
        glm::vec4 p_destiny = this->target->position;

        this->position = glm::vec4(glm::vec3(p_origin - p_destiny) * this->time, 1.0f);
    }
}
