#include "engine/resource_manager.hpp"
#include "engine/matrices.hpp"
#include "model/ammo/laser.hpp"

void Laser::build()
{
    // Initialize object, shaders, and textures
    ResourceManager::load_object("../../src/objects/cube.obj", this, this->name);
    this->shader = ResourceManager::load_shader("../../src/shaders/default_lighting.vs", "../../src/shaders/default_lighting.fs", nullptr, this->name);
    this->shader.use();

    return;
}

void Laser::render(glm::mat4 view, glm::mat4 projection)
{
    if (!this->completed_movement()) {
        // Set to use this shader
        this->shader.use();

        // Configure view and projection matrices
        this->shader.set_matrix("view", view);
        this->shader.set_matrix("projection", projection);

        // Bind the VAO
        glBindVertexArray(this->VAO);

        // Configure the color
        this->shader.set_vector("color_v", glm::vec4(0.14f, 1.0f, 0.91f, 0.3f));

        // Calculate the model matrix
        glm::mat4 model = matrix::identity_matrix(); // make sure to initialize matrix to identity matrix first
        model *= matrix::translate_matrix(this->position);
        model *= matrix::rotate_y_matrix(this->angle);                          // Hard atan2 computations can be seen in Laser#update
        model *= matrix::rotate_x_matrix(-this->x_angle);                        // Hard atan2 computations can be seen in Laser#update
        model *= matrix::translate_matrix(0.0f, 0.0f, this->scale.z / 2);      // Fix distortion caused by the translation
        model *= matrix::scale_matrix(this->scale);                             // Scale according to what is defined in Laser#update
        this->shader.set_matrix("model", model);

        // Draw the element
        glDrawElements(this->drawMode, this->indexesLength, GL_UNSIGNED_INT, this->indexesOffset);

        // Unbind the VAO to prevent bugs
        glBindVertexArray(0);
    } else {
        printf("[WARN] TODO: You should delete me! %s\n", this->name.c_str());
    }


}

// Calculate the ray angle
void Laser::update(float delta_time){
    if (!this->completed_movement() && this->target != nullptr) { // Keep updating the laser direction and angle
        this->time += delta_time * this->speed;

        // Compute the middle of the target for the target_position
        glm::vec4 target_position = this->target->position;
        target_position.y = this->target->position.y + (this->target->bbox_max.y - this->target->bbox_min.y) / 2;

        // The scale of this cube is the distance between the objects. In the render we only expand on the X axis
        this->scale = glm::vec3(this->time / 5.0f, this->time / 5.0f, matrix::norm(target_position - this->origin_position));

        // The y rotation of this ray is related to the equation below
        this->angle = std::atan2(target_position.x - this->origin_position.x, target_position.z - this->origin_position.z);

        // The x rotation of this ray is related to the equation below
        this->x_angle = std::atan2(target_position.y - this->origin_position.y, target_position.z - this->origin_position.z);
    }
}
