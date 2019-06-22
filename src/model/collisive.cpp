#include "model/collisive.hpp"
#include "engine/matrices.hpp"

bool Collisive::collide(glm::vec3 bbox_min, glm::vec3 bbox_max) {

    // Calculate this object bbox translated and scaled
    // OBS: We don't rotate them. That is a bug needed, so that our rotations dont fuck up with our algorithm
    glm::vec3 this_bbox_min = matrix::translate_matrix(this->position) * matrix::scale_matrix(this->scale) * glm::vec4(this->bbox_min, 1.0f);
    glm::vec3 this_bbox_max = matrix::translate_matrix(this->position) * matrix::scale_matrix(this->scale) * glm::vec4(this->bbox_max, 1.0f);

    // Check the collisions
    if (this_bbox_max.x < bbox_min.x || this_bbox_min.x > bbox_max.x) return false;
    if (this_bbox_max.y < bbox_min.y || this_bbox_min.y > bbox_max.y) return false;
    if (this_bbox_max.z < bbox_min.z || this_bbox_min.z > bbox_max.z) return false;

    return true;
}

bool Collisive::collide(Collisive *collisive) {

    if (!this->collisive)
        return false;

    // As we passed the object, we need to update its bbox
    // OBS: We don't rotate them. That is a bug needed, so that our rotations dont fuck up with our algorithm
    glm::vec3 collisive_bbox_min = matrix::translate_matrix(collisive->position) * matrix::scale_matrix(collisive->scale) * glm::vec4(collisive->bbox_min, 1.0);
    glm::vec3 collisive_bbox_max = matrix::translate_matrix(collisive->position) * matrix::scale_matrix(collisive->scale) * glm::vec4(collisive->bbox_max, 1.0);

    return collide(collisive_bbox_min, collisive_bbox_max);
}

void Collisive::set_collisive() {
    this->collisive = true;
}

void Collisive::unset_collisive() {
    this->collisive = false;
}
