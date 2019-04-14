#include "engine\camera.hpp"
#include "engine\matrices.hpp"

glm::mat4 Camera::get_view_matrix()
{
    return matrix::camera_view_matrix(position, front, up);
}

void Camera::process_keyboard(CameraMovement direction, float delta_time)
{
    float velocity = movement_speed * delta_time;
    if (direction == FORWARD)
        position += front * velocity;
    if (direction == BACKWARD)
        position -= front * velocity;
    if (direction == LEFT)
        position -= right * velocity;
    if (direction == RIGHT)
        position += right * velocity;

    // Make sure the user stays at the ground level
    position.y = 0.0f; // <-- this one-liner keeps the user at the ground level (xz plane)
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
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
    }

    // Update Front, Right and Up Vectors using the updated Euler angles
    update_camera_vectors();
}

void Camera::process_mouse_scroll(float yoffset)
{
    if (zoom >= 1.0f && zoom <= 45.0f)
        zoom -= yoffset;

    if (zoom <= 1.0f)
        zoom = 1.0f;
    if (zoom >= 45.0f)
        zoom = 45.0f;
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
