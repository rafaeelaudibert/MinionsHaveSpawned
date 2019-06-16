#include "model/collisive.hpp"
#include "engine/matrices.hpp"

bool Collisive::collide(glm::vec3 bbox_min, glm::vec3 bbox_max) {

    // If the "highest" part is "lower" than the "lowest" other object's part, or;
    // If the "lowest" part is "higher" than the "highest" other object's part
    //matrix::print_vector(glm::vec4(bbox_min, 0.0f), "bbox_min");
    //matrix::print_vector(glm::vec4(bbox_max, 0.0f), "bbox_max");
    //matrix::print_vector(glm::vec4(this->bbox_min + this->position, 0.0f), "this->bbox_min");
    //matrix::print_vector(glm::vec4(this->bbox_max + this->position, 0.0f), "this->bbox_max");
    if (this->bbox_max.x + this->position.x < bbox_min.x || this->bbox_min.x + this->position.x > bbox_max.x) return false;
    if (this->bbox_max.y + this->position.y < bbox_min.y || this->bbox_min.y + this->position.y > bbox_max.y) return false;
    if (this->bbox_max.z + this->position.z < bbox_min.z || this->bbox_min.z + this->position.z > bbox_max.z) return false;

    return true;
}

bool Collisive::collide(Collisive *collisive) {
    return collide(collisive->bbox_min, collisive->bbox_max);
}
