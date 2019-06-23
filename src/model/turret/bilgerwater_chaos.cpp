#include "engine/resource_manager.hpp"
#include "engine/matrices.hpp"
#include "model/turret/bilgerwater_chaos.hpp"

void BilgerwaterChaos::build()
{
    // Initialize object, shaders, and textures
    ResourceManager::load_object("../../src/objects/bw_chaos_turret.obj", this, this->name);
    this->shader = ResourceManager::load_shader("../../src/shaders/turret.vs", "../../src/shaders/turret.fs", nullptr, this->name);
    this->shader.use();

    switch (this->color)
    {
    case TurretColor::RED:
    case TurretColor::BLUE:
        this->texture = ResourceManager::load_texture("../../src/textures/bw_chaos_turret.jpg", this->name);
        break;
    default:
        throw std::runtime_error("[ERROR] Error when trying to load the BilgerwaterChaos model - Non existent TurretColor.");
    }

    // Configure shooting position
    this->shooting_position = glm::vec4(0.0f, 3.78f, 1.59f, 1.0f);

    return;
}

void BilgerwaterChaos::render(glm::mat4 view, glm::mat4 projection)
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

    // Configure the shader variables
    this->shader.set("placed", this->placed);
    this->shader.set("can_place", this->can_place());

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

    // If there is an ammo to render, render it
    if (this->ammo != nullptr) {
        this->ammo->render(view, projection);
    }

}

void BilgerwaterChaos::update(float delta_time){

    // It is not shooting anybody
    if (this->ammo == nullptr) {
        this->ellapsed_time += delta_time;    // Count the time it spent recharging

        // If can already attack
        if (this->ellapsed_time >= this->recharge_time)
        {
            if (this->target == nullptr || this->target->is_dead() || matrix::norm(this->target->position - this->shooting_position) > this->AGGRO_RADIUS) {  // It doesn't have a valid target

                // Find a valid target
                glm::vec4 translated_shooting_position = matrix::translate_matrix(this->position) * matrix::scale_matrix(this->scale) * matrix::rotate_matrix(this->angle, this->orientation) * this->shooting_position;
                Enemy *closest_enemy = nullptr;
                float distance = Constants::MAX_NUMBER;
                for (const auto &object : Game::enemy_objects)
                {
                    float enemy_distance = matrix::norm(object.second->position - translated_shooting_position);
                    if (!object.second->is_dead() && enemy_distance < this->AGGRO_RADIUS && enemy_distance <= distance) {
                        distance = enemy_distance;
                        closest_enemy = object.second;
                    }
                }

                this->target = closest_enemy; // Update the target (event if it didn't find, it will work
            }

            if (this->target != nullptr) {  // Can start to shoot in that target

                // Rotate to point to the target
                this->angle = std::atan2(this->target->position.x - this->position.x, this->target->position.z - this->position.z);

                this->ammo = new CannonBall(this->name + "_cannon_ball",
                                            this->target, 1.5f, 5.0f,
                                            AmmoEffects::AreaDamageEffect(),
                                            matrix::translate_matrix(this->position) * matrix::scale_matrix(this->scale) * matrix::rotate_matrix(this->angle, this->orientation) * this->shooting_position);
            }
        }

    } else { // It is already shooting someone
        this->ammo->update(delta_time);

         // If already completed its movement, we hit the enemy, and delete it
         if (this->ammo->completed_movement()) {
             this->ammo->hit_enemy();
             delete this->ammo;
             this->ammo = nullptr;
             this->ellapsed_time = 0.0f;
        }
    }
}

