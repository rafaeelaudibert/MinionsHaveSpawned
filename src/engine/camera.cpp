#include <algorithm>
#include "engine\camera.hpp"
#include "engine\matrices.hpp"
#include "utils\helpers.hpp"


// Self explicative
glm::mat4 Camera::get_view_matrix()
{
    return matrix::camera_view_matrix(position, get_front_vector(), up);
}

// Make the front vector calculation easier, due to the two types of camera
glm::vec4 Camera::get_front_vector()
{
    if (this->camera_type == CameraType::LOOK_AROUND)
    {
        return matrix::normalize(front);
    }
    else
    {
        return matrix::normalize(this->look_at_target->position - this->position);
    }
}

// Processes keyboard pressing, according to the camera type
bool Camera::process_movement(CameraMovement direction, float delta_time, std::map<std::string, Collisive *> collisive_objects, const Game &game)
{
    float velocity = movement_speed * delta_time;
    glm::vec4 new_position = position;
    glm::vec4 old_position = position;

    if (this->camera_type == CameraType::LOOK_AROUND)
    {
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
    }
    else
    {
        glm::vec4 position_on_ground = glm::vec4(this->position.x, 0.0f, this->position.z, this->position.w);

        // The signals are inverted because we are generating "left" vectors, instead of right ones
        if (direction == FORWARD)
            new_position -= matrix::crossproduct(up, matrix::crossproduct(up, matrix::normalize(this->look_at_target->position - position_on_ground))) * velocity;
        if (direction == BACKWARD)
            new_position += matrix::crossproduct(up, matrix::crossproduct(up, matrix::normalize(this->look_at_target->position - position_on_ground))) * velocity;
        if (direction == LEFT)
            new_position += matrix::crossproduct(up, matrix::normalize(this->look_at_target->position - position_on_ground)) * velocity;
        if (direction == RIGHT)
            new_position -= matrix::crossproduct(up, matrix::normalize(this->look_at_target->position - position_on_ground)) * velocity;
        if (direction == DOWN)
            new_position.y = new_position.y + game.y_speed * delta_time;
    }


    // Update camera bbox to collision checking
    set_bbox(new_position);

    bool collision = check_collision(collisive_objects);
    if (!collision)
    {
        position = new_position; // Configure the new position
        return true;
    }
    else
    {
        set_bbox(old_position); // Restore old bounding box
        return false;
    }
}

// Proccess x and y axis mouse movement
void Camera::process_mouse_movement(float xoffset, float yoffset, GLboolean constrain_pitch)
{
    // Only move the camera around if we are in LookAround mode
    if (this->camera_type == CameraType::LOOK_AROUND)
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
}

// Process zoom
void Camera::process_mouse_scroll(float yoffset)
{
    zoom = utils::clamping(zoom - yoffset, 45.0f, 1.0f);
}

// Self explicative
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

// Check if the cammera collided with all the collisive objects
bool Camera::check_collision(std::map<std::string, Collisive *> collisive_objects)
{
    return std::any_of(collisive_objects.begin(),
                       collisive_objects.end(),
                       [this](std::pair<std::string, Collisive *> entry)
    {
        return check_collision(entry.second);
    });
}

// Check if the camera collided
bool Camera::check_collision(Collisive * object)
{
    return object->collide(this->camera_bbox_min, this->camera_bbox_max);
}

// Update camera bbox
void Camera::set_bbox(glm::vec4 position)
{
    this->camera_bbox_min = glm::vec3(position.x - CAMERA_WIDTH, position.y - Game::character_height, position.z - CAMERA_DEPTH);
    this->camera_bbox_max = glm::vec3(position.x + CAMERA_WIDTH, position.y + CAMERA_HEIGHT, position.z + CAMERA_DEPTH);
}

// Change between camera types
void Camera::switch_camera_type()
{
    if (this->camera_type == CameraType::LOOK_AT)
    {
        // Update front, pitch and yaw to have a smooth transition
        this->front = this->look_at_target->position - this->position;
        this->front.w = 0.0f;
        this->front = matrix::normalize(front);
        this->pitch = glm::degrees(std::asin(this->front.y));
        this->yaw = glm::degrees(std::asin(this->front.z / std::cos(glm::radians(this->pitch))));

        this->camera_type = CameraType::LOOK_AROUND; // Easy switch
        this->look_at_target = nullptr;
        printf("[GAME] Camera changed to LOOK_AROUND\n");
    }
    else
    {
        // Only change if there is a minion to target
        if (Game::enemy_objects.size() != 0)
        {
            // Change to the closest minion to the order nexus (ally one)
            float min_distance = Constants::MAX_NUMBER;
            Nexus *nexus = Game::order_nexus;
            Enemy *target = Game::enemy_objects.begin()->second;
            for (const auto &object : Game::enemy_objects)
            {
                float distance = matrix::norm(object.second->position - nexus->position);
                if (!object.second->is_dead() && distance <= min_distance)
                {
                    min_distance = distance;
                    target = object.second;
                }
            }
            this->look_at_target = target;
            this->camera_type = CameraType::LOOK_AT;
            printf("[GAME] Camera changed to LOOK_AT, focusing %s\n", this->look_at_target->name.c_str());
        }
    }

    // Update camera vectors
    this->update_camera_vectors();
}
