#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include "engine/matrices.hpp"

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum CameraMovement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    // Camera Attributes
    glm::vec4 position;
    glm::vec4 front;
    glm::vec4 up;
    glm::vec4 right;
    glm::vec4 world_up;

    // Euler Angles
    float yaw;
    float pitch;

    // Camera options
    float movement_speed;
    float mouse_sensitivity;
    float zoom;

    // Constructor with vectors
    Camera(glm::vec4 position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), glm::vec4 up = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : front(glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)), movement_speed(SPEED), mouse_sensitivity(SENSITIVITY), zoom(ZOOM)
    {
        this->position = position;
        this->up = up;
        this->world_up = up;
        this->yaw = yaw;
        this->pitch = pitch;
        update_camera_vectors();
    }
    // Constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : front(glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)), movement_speed(SPEED), mouse_sensitivity(SENSITIVITY), zoom(ZOOM)
    {
        this->position = glm::vec4(posX, posY, posZ, 1.0);
        this->up = glm::vec4(upX, upY, upZ, 0.0);
        this->world_up = glm::vec4(upX, upY, upZ, 0.0);
        this->yaw = yaw;
        this->pitch = pitch;
        update_camera_vectors();
    }

    // Returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 get_view_matrix();

    // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void process_keyboard(CameraMovement direction, float delta_time);

    // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void process_mouse_movement(float xoffset, float yoffset, GLboolean constrain_pitch = GL_TRUE);

    // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void process_mouse_scroll(float yoffset);

private:
    // Calculates the front vector from the Camera's (updated) Euler Angles
    void update_camera_vectors();
};
#endif
