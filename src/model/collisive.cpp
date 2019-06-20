#include "model/collisive.hpp"
#include "engine/matrices.hpp"

bool Collisive::collide(glm::vec3 bbox_min, glm::vec3 bbox_max) {

    // Calculate this object bbox translated, scaled, and rotated
    glm::vec3 this_bbox_min = this->position + matrix::scale_matrix(this->scale) * matrix::rotate_matrix(this->angle, this->orientation) * glm::vec4(this->bbox_min, 1.0f);
    glm::vec3 this_bbox_max = this->position + matrix::scale_matrix(this->scale) * matrix::rotate_matrix(this->angle, this->orientation) * glm::vec4(this->bbox_max, 1.0f);

    // Check the collisions
    if (this_bbox_max.x < bbox_min.x || this_bbox_min.x > bbox_max.x) return false;
    if (this_bbox_max.y < bbox_min.y || this_bbox_min.y > bbox_max.y) return false;
    if (this_bbox_max.z < bbox_min.z || this_bbox_min.z > bbox_max.z) return false;

    return true;
}

bool Collisive::collide(Collisive *collisive) {
    return this->collisive && collide(collisive->bbox_min, collisive->bbox_max);
}

void Collisive::set_collisive() {
    this->collisive = true;
}

void Collisive::unset_collisive() {
    this->collisive = false;
}
