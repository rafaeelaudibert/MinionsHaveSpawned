#include "engine\camera.hpp"
#include "engine\matrices.hpp"
#include "utils\helpers.hpp"

glm::mat4 Camera::get_view_matrix()
{
    return matrix::camera_view_matrix(position, front, up);
}

void Camera::process_keyboard(CameraMovement direction, float delta_time)
{
    float velocity = movement_speed * delta_time;
    float old_position_y = position.y;

    if (direction == FORWARD)
        position += front * velocity;
    if (direction == BACKWARD)
        position -= front * velocity;
    if (direction == LEFT)
        position -= right * velocity;
    if (direction == RIGHT)
        position += right * velocity;

    // Prevent jumps abusing from the front * velocity multiplication
    position.y = old_position_y;
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
        pitch = utils::clamping(pitch, 89.0f, -89.0f);
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
    this->front = normalize(front);

    // Also re-calculate the Right and Up vector
    // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    this->right = matrix::normalize(matrix::crossproduct(this->front, this->world_up));
    this->up = matrix::normalize(matrix::crossproduct(this->right, this->front));
}
