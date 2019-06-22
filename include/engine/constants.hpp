#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <limits>

namespace Constants {

    // Static constant variables
    const float MAP_SIZE = 80.0f;                                   // Width/Depth of the map
    const float GRAVITY = -26.0f;                                   // Gravity Force
    const float MINIMUM_HEIGHT = -150.0f;                           // Minimum height the camera can be
    const float MAX_FALLING_SPEED = -180.0f;                        // Maximum falling speed due to air resistance
    const float JUMP_SPEED = 12.5f;                                 // Instantaneous jump speed
    const float CROUCHING_SPEED = -3.5f;                            // Instantaneous crouching speed
    const float CHARACTER_STANDING_HEIGHT = 3.0f;                   // Height of the character while standing, in meters
    const float CHARACTER_CROUCHING_HEIGHT = 2.3f;                  // Height of the character while crouching, in meters
    const float CROUCHING_SPEED_MULTIPLIER = 0.4f;                  // How much of the normal speed the player runs while crouching
    const float SPEED = 10.0f;                                      // Player speed
    const float HAND_LESS_HEIGHT = 1.2f;                            // How lower is the hand position according to the camera
    const float SPAWNING_TURRET_MAX_DISTANCE = 12.0f;               // Maximum distance which a player can drop a turret
    const float EPSILON = std::numeric_limits<float>::epsilon();    // Machine epsilon
    const float MAX_NUMBER = std::numeric_limits<float>::max();     // Machine maximum number
}

#endif // CONSTANTS_H




