#include <algorithm>
#include "engine\camera.hpp"
#include "engine\matrices.hpp"
#include "utils\helpers.hpp"

glm::mat4 Camera::get_view_matrix()
{
    return matrix::camera_view_matrix(position, front, up);
}

bool Camera::process_movement(CameraMovement direction, float delta_time, std::map<std::string, Collisive *> collisive_objects, const Game &game)
{
    float velocity = movement_speed * delta_time;
    glm::vec4 new_position = position;
    glm::vec4 old_position = position;

    if (direction == FORWARD)
        new_position += matrix::crossproduct(up, right) * velocity;
    if (direction == BACKWARD)
        new_position -= matrix::crossproduct(up, right) * velocity;
    if (direction == LEFT)
        new_position -= right * velocity;
    if (direction == RIGHT)
        new_position += right * velocity;
    if (direction == DOWN)
        new_position.y = new_position.y + game.y_speed * delta_time;

    // Update camera bbox to collision checking
    set_bbox(new_position);

    bool collision = check_collision(collisive_objects);
    if (!collision) {
        position = new_position; // Configure the new position
        return true;
    } else {
        set_bbox(old_position); // Restore old bounding box
        return false;
    }
}

void Camera::process_mouse_movement(float xoffset, float yoffset, GLboolean constrain_pitch)
{
    xoffset *= mouse_sensitivity;
    yoffset *= mouse_sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrain_pitch)
    {
        pitch = utils::clamping(pitch, 89.9f, -89.9f);
    }

    // Update Front, Right and Up Vectors using the updated Euler angles
    update_camera_vectors();
}

void Camera::process_mouse_scroll(float yoffset)
{
    zoom = utils::clamping(zoom - yoffset, 45.0f, 1.0f);
}

void Camera::update_camera_vectors()
{

    // Calculate the new Front vector
    glm::vec4 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.w = 0.0f;
    this->front = matrix::normalize(front);

    // Also re-calculate the Right  vector
    // Normalize the vector, because their length gets closer to 0 the more you look up or down which results in slower movement.
    this->right = matrix::normalize(matrix::crossproduct(this->front, this->world_up));
}

bool Camera::check_collision(std::map<std::string, Collisive *> collisive_objects) {
    return std::any_of(collisive_objects.begin(),
                       collisive_objects.end(),
                       [this](std::pair<std::string, Collisive *> entry) {
                           return check_collision(entry.second);
                       });
}

bool Camera::check_collision(Collisive * object) {
    return object->collide(this->camera_bbox_min, this->camera_bbox_max);
}

void Camera::set_bbox(glm::vec4 position) {
    this->camera_bbox_min = glm::vec3(position.x - CAMERA_WIDTH, position.y - Game::character_height, position.z - CAMERA_DEPTH);
    this->camera_bbox_max = glm::vec3(position.x + CAMERA_WIDTH, position.y + CAMERA_HEIGHT, position.z + CAMERA_DEPTH);
}
