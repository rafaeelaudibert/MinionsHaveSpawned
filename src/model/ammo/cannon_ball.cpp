#include "engine/resource_manager.hpp"
#include "engine/matrices.hpp"
#include "model/ammo/cannon_ball.hpp"

void CannonBall::build()
{
    // Initialize object, shaders, and textures
    ResourceManager::load_object("../../src/objects/cannon_ball.obj", this, this->name);
    this->shader = ResourceManager::load_shader("../../src/shaders/cannon_ball.vs", "../../src/shaders/cannon_ball.fs", nullptr, this->name);
    this->shader.use();

    this->texture = ResourceManager::load_texture("../../src/textures/cannon_ball.jpg", this->name);

    return;
}

void CannonBall::render(glm::mat4 view, glm::mat4 projection)
{
    if (!this->completed_movement()){
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
    } else {
        printf("[WARN] TODO: You should delete me! %s\n", this->name.c_str());
    }

}

// Calculate the bezier curve movement for the ball
void CannonBall::update(float delta_time){
    if (this->completed_movement()) {   // Already finished moving
        this->hit_enemy();
    } else if (this->target != nullptr) { // Keep moving the ball, using bezier curves, towards the enemy
        this->time += delta_time * this->speed;

        glm::vec4 p_origin = this->origin_position;
        glm::vec4 p_destiny = this->target->position;
        glm::vec4 p_control = glm::vec4(p_destiny.x, p_origin.y, p_destiny.z, 1.0f);

        glm::vec4 p1 = glm::vec4(glm::vec3(p_control - p_origin) * this->time, 1.0f);
        glm::vec4 p2 = glm::vec4(glm::vec3(p_destiny - p_control) * this->time, 1.0f);
        this->position = glm::vec4(glm::vec3(p2 - p1) * this->time, 1.0f);
    }
}
