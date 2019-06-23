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
        this->shader.set_vector("color_v", glm::vec4(0.14f, 1.0f, 0.91f, 1.0f));

        // Calculate the model matrix
        glm::mat4 model = matrix::identity_matrix(); // make sure to initialize matrix to identity matrix first
        model *= matrix::translate_matrix(this->position);
        model *= matrix::scale_matrix(this->scale);
        model *= matrix::rotate_matrix(this->angle, this->orientation);         // Hard atan2 computation can be seen below boys
        model *= matrix::translate_matrix(glm::vec4(0.5f, 0.0f, 0.0f, 0.0f));   // Make the cube centered in the left face
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
void Laser::update(float delta_time){
    if (this->completed_movement()) {   // Already finished moving
        this->hit_enemy();
    } else if (this->target != nullptr) { // Keep updating the laser direction and angle
        this->time += delta_time * this->speed;

        // The scale of this cube is the distance between the objects. In the render we only expand on the X axis
        this->scale = glm::vec3(matrix::norm(this->target->position - this->origin_position), this->time, this->time);

        // The y rotation of this ray is related to the equation below
        this->angle = std::atan2(this->target->position.x - this->origin_position.x, this->target->position.z - this->origin_position.z);
    }
}
