#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <map>

#include "engine/camera.hpp"
#include "model/base.hpp"
#include "model/collisive.hpp"
#include "model/enemy.hpp"
#include "model/turret.hpp"
#include "model/hand.hpp"
#include "model/nexus.hpp"

// Represents the current state of the game
enum GameState
{
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN,
    GAME_LOSE
};

// Represents the player status
enum PlayerStatus
{
    STANDING,
    CROUCHING,
    UNCROUCHING,
    JUMPING
};



// Game holds all game-related state and functionality.
// Combines all game-related data into a single class for
// easy access to each of the components and manageability.
class Game
{
public:
    // GameState variables
    static GameState state;
    GLboolean keys[1024] = {false};
    GLuint width, height;
    float screen_ratio;

    // Mechanic variables
    static float character_height;                              // Height of the character, in meters
    PlayerStatus player_status = PlayerStatus::STANDING;        // Current player status
    GLfloat y_speed = 0;                                        // Current character falling velocity
    glm::vec4 aiming_position;

    // Time variables
    GLfloat lastFrame = 0.0f;
    GLfloat deltaTime = 0.0f;

    // Camera
    static Camera camera;

    // Objects map
    std::map<std::string, GameObject *> objects;

    // Holding on hand turret
    Turret *hand_turret;

    // Hand
    Hand *hand;

    // Game nexus
    Nexus *order_nexus, *chaos_nexus;

    // Collisive objects map
    std::map<std::string, Collisive *> collisive_objects;

    // Enemy objects map
    std::map<std::string, Enemy *> enemy_objects;

    // Constructor/Destructor
    Game(GLuint width, GLuint height);
    ~Game();

    // Initialize game state (load all shaders/textures/levels)
    void init();

    // GameLoop helpers
    void new_frame();
    void update();
    void render();

    // Processes game input
    void process_input();
};

#endif
