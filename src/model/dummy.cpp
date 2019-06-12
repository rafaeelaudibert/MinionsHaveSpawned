#include "engine/resource_manager.hpp"
#include "engine/matrices.hpp"
#include "model/dummy.hpp"

void Dummy::build()
{

    // poro.obj e poro.jpg
    // elder_dragon.obj e sru_dragon_elder_tx_cm.png
    // ha_chaosminion_melee.obj e chaosminion_melee_tx_cm.jpg
    // ha_chaosminion_ranged.obj e chaosminion_ranged_tx_cm.jpg
    // ha_chaosminion_siege.obj e chaosminion_siege_tx_cm.jpg
    // ha_chaosminion_super.obj e chaosminion_super_tx_cm.jpg
    // ha_chaosturret.obj e ha_ap_chaosturret_blue/red.jpg
    // order_turret_1.obj e sruap_turret_order1_tx_cm.jpg
    // zzrot_minion.obj e zzrot_minion_tx_cm.jpg

    // Initialize object, shaders, and textures
    ResourceManager::load_object("../../src/objects/elder_dragon.obj", this, this->name);
    this->shader = ResourceManager::load_shader("../../src/shaders/default_texture.vs", "../../src/shaders/default_texture.fs", nullptr, this->name);
    this->shader.use();

    this->texture = ResourceManager::load_texture("../../src/textures/sru_dragon_elder_tx_cm.jpg", this->name);

    return;
}

void Dummy::render(glm::mat4 view, glm::mat4 projection)
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
