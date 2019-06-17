#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace Constants {

    // Static constant variables
    const float GRAVITY = -26.0f;                    // Gravity Force
    const float MAX_FALLING_SPEED = -180.0f;         // Maximum falling speed due to air resistance
    const float JUMP_SPEED = 12.5f;                  // Instantaneous jump speed
    const float CROUCHING_SPEED = -3.5f;             // Instantaneous crouching speed
    const float CHARACTER_STANDING_HEIGHT = 3.0f;    // Height of the character while standing, in meters
    const float CHARACTER_CROUCHING_HEIGHT = 2.3f;   // Height of the character while crouching, in meters
    const float CROUCHING_SPEED_MULTIPLIER = 0.4f;   // How much of the normal speed the player runs while crouching
    const float SPEED = 10.0f;                       // Player speed
}

#endif // CONSTANTS_H




