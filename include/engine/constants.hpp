#include <glm/glm.hpp>

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <limits>

// Constants used throughout the game
namespace Constants {

    // Static constant variables
    const float MAP_SIZE = 80.0f;                                                   // Width/Depth of the map
    const float GRAVITY = -26.0f;                                                   // Gravity Force
    const float MINIMUM_HEIGHT = -150.0f;                                           // Minimum height the camera can be
    const float MAX_FALLING_SPEED = -180.0f;                                        // Maximum falling speed due to air resistance
    const float JUMP_SPEED = 12.5f;                                                 // Instantaneous jump speed
    const float CROUCHING_SPEED = -3.5f;                                            // Instantaneous crouching speed
    const float CHARACTER_STANDING_HEIGHT = 3.0f;                                   // Height of the character while standing, in meters
    const float CHARACTER_CROUCHING_HEIGHT = 2.3f;                                  // Height of the character while crouching, in meters
    const float CROUCHING_SPEED_MULTIPLIER = 0.4f;                                  // How much of the normal speed the player runs while crouching
    const float SPEED = 10.0f;                                                      // Player speed
    const float HAND_LESS_HEIGHT = 1.2f;                                            // How lower is the hand position according to the camera
    const float SPAWNING_TURRET_MAX_DISTANCE = 12.0f;                               // Maximum distance which a player can drop a turret
    const float MINION_DEATH_SPEED = 1.0f;                                          // Speed which the minions goes beneath the ground after dead
    const float SPAWN_RATE = 1.5f;                                                  // How many seconds between each minion spawn
    const float START_GOLD = 100.0f;                                                // Game gold start quantity
    const float GOLD_PER_WAVE = 75.0f;                                              // Gold earned each time a wave finishes
    const float MINION_LIFE_DAMAGE_MULTIPLIER = 1.75f;                              // How many times more is the minion damage on the nexus according to his life
    const float MINION_GIGGLES = 80.0f;                                             // How many times a minion "giggles" in each step of the lane walking
    const glm::vec4 CHAOS_NEXUS_POSITION = glm::vec4(33.0f, 0.0f, -33.25f, 1.0f);   // Order nexus position in game
    const glm::vec4 ORDER_NEXUS_POSITION = glm::vec4(-33.5f, 0.0f, 32.4f, 1.0f);    // Chaos nexus position in game
    const float EPSILON = std::numeric_limits<float>::epsilon();                    // Machine epsilon
    const float MAX_NUMBER = std::numeric_limits<float>::max();                     // Machine maximum number

}

#endif // CONSTANTS_H




